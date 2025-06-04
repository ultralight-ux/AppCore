# build-shaders.ps1
# Windows-only shader build system for AppCore
# Compiles HLSL shaders to D3D11, SPIR-V, GLSL, and Metal formats

param(
    [switch]$Debug = $false,
    [switch]$Clean = $false,
    [string]$OutputDir = "../generated",
    [string]$ToolsDir = "./tools"
)

$ErrorActionPreference = "Stop"

# Shader definitions
$VertexShaders = @(
    @{Name="vertex_path"; File="../hlsl/vertex_path.hlsl"},
    @{Name="vertex_quad"; File="../hlsl/vertex_quad.hlsl"}
)

$PixelShaders = @(
    @{Name="fill"; File="../hlsl/fill.hlsl"},
    @{Name="fill_path"; File="../hlsl/fill_path.hlsl"},
    @{Name="filter_basic"; File="../hlsl/filter_basic.hlsl"},
    @{Name="filter_blur"; File="../hlsl/filter_blur.hlsl"},
    @{Name="filter_dropshadow"; File="../hlsl/filter_dropshadow.hlsl"}
)

# Tool paths - using local tools
$FXC = "../tools/fxc.exe"
$DXC = "../tools/dxc/bin/x64/dxc.exe"
$SPIRV_CROSS = "../tools/spirv-cross.exe"

# Download DXC if not present
function Download-DXC {
    $dxcDir = "../tools/dxc"
    if (Test-Path $dxcDir) {
        return
    }
    
    Write-Host "Downloading DXC from GitHub releases..." -ForegroundColor Yellow
    
    $dxcUrl = "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2505/dxc_2025_05_24.zip"
    $dxcZip = "../tools/dxc.zip"
    
    try {
        # Download DXC
        Invoke-WebRequest -Uri $dxcUrl -OutFile $dxcZip -UserAgent "Mozilla/5.0"
        
        # Extract to tools directory
        Expand-Archive -Path $dxcZip -DestinationPath $dxcDir -Force
        
        # Clean up zip
        Remove-Item $dxcZip -Force
        
        Write-Host "DXC downloaded and extracted successfully" -ForegroundColor Green
    }
    catch {
        throw "Failed to download DXC: $_"
    }
}

# Create output directories
function Initialize-OutputDirectories {
    $dirs = @(
        $OutputDir,
        "$OutputDir/temp",           # Temporary files (not committed)
        "$OutputDir/metal",          # Metal source files (for macOS)
        "$OutputDir/headers",
        "$OutputDir/headers/d3d11",  # D3D11 binary headers (Windows)
        "$OutputDir/headers/glsl"    # GLSL text headers (Linux)
    )
    
    foreach ($dir in $dirs) {
        if (!(Test-Path $dir)) {
            New-Item -ItemType Directory -Path $dir -Force | Out-Null
        }
    }
}

# Clean output directories
function Clean-Output {
    if (Test-Path $OutputDir) {
        Remove-Item -Path $OutputDir -Recurse -Force
        Write-Host "Cleaned output directory: $OutputDir" -ForegroundColor Green
    }
}

# Check if tools exist
function Verify-Tools {
    Write-Host "Verifying tools..." -ForegroundColor Cyan
    
    # Check local FXC
    if (!(Test-Path $FXC)) {
        throw "FXC not found at $FXC. Please copy fxc.exe to ../tools/"
    }
    Write-Host "Found FXC: $FXC" -ForegroundColor Green
    
    # Download DXC if needed
    Download-DXC
    
    # Check DXC
    if (!(Test-Path $DXC)) {
        throw "DXC still not found at $DXC after download attempt"
    }
    Write-Host "Found DXC: $DXC" -ForegroundColor Green
    
    # Check local SPIRV-Cross
    if (!(Test-Path $SPIRV_CROSS)) {
        throw "SPIRV-Cross not found at $SPIRV_CROSS. Please copy spirv-cross.exe to ../tools/"
    }
    Write-Host "Found SPIRV-Cross: $SPIRV_CROSS" -ForegroundColor Green
}

# Compile shader with FXC (D3D11)
function Compile-D3D11 {
    param(
        [string]$InputFile,
        [string]$OutputFile,
        [string]$Profile,
        [string]$EntryPoint = "main"
    )
    
    $args = @(
        "/T", $Profile,
        "/E", $EntryPoint,
        "/I", "../hlsl",
        "/Fo", $OutputFile,
        $InputFile
    )
    
    if ($Debug) {
        $args += "/Od", "/Zi"
    } else {
        $args += "/O2"
    }
    
    & $FXC $args
    if ($LASTEXITCODE -ne 0) {
        throw "FXC compilation failed for $InputFile"
    }
}

# Compile shader with DXC to SPIR-V
function Compile-SPIRV {
    param(
        [string]$InputFile,
        [string]$OutputFile,
        [string]$Profile,
        [string]$EntryPoint = "main"
    )
    
    $args = @(
        "-T", $Profile,
        "-E", $EntryPoint,
        "-spirv",
        "-fspv-target-env=vulkan1.1",
        "-I", "../hlsl",
        "-Fo", $OutputFile,
        $InputFile
    )
    
    if ($Debug) {
        $args += "-Od"
    } else {
        $args += "-O3"
    }
    
    & $DXC $args
    if ($LASTEXITCODE -ne 0) {
        throw "DXC SPIR-V compilation failed for $InputFile"
    }
}

# Cross-compile SPIR-V to GLSL
function Compile-GLSL {
    param(
        [string]$InputFile,
        [string]$OutputFile
    )
    
    $args = @(
        $InputFile,
        "--version", "330",
        "--no-420pack-extension",
        "--remove-unused-variables",
        "--output", $OutputFile
    )
    
    & $SPIRV_CROSS $args
    if ($LASTEXITCODE -ne 0) {
        throw "SPIRV-Cross GLSL compilation failed for $InputFile"
    }
}

# Cross-compile SPIR-V to Metal
function Compile-Metal {
    param(
        [string]$InputFile,
        [string]$OutputFile
    )
    
    $args = @(
        $InputFile,
        "--msl",
        "--msl-version", "20100",
        "--msl-argument-buffers",
        "--output", $OutputFile
    )
    
    & $SPIRV_CROSS $args
    if ($LASTEXITCODE -ne 0) {
        throw "SPIRV-Cross Metal compilation failed for $InputFile"
    }
}

# Generate C header from binary data
function Generate-BinaryHeader {
    param(
        [string]$InputFile,
        [string]$OutputFile,
        [string]$VariableName
    )
    
    $bytes = [System.IO.File]::ReadAllBytes($InputFile)
    $content = @"
// Generated shader header for $VariableName
// Generated from: $(Split-Path $InputFile -Leaf)
// Size: $($bytes.Length) bytes

#pragma once

static const unsigned char ${VariableName}_data[] = {
"@
    
    # Write bytes in rows of 16
    for ($i = 0; $i -lt $bytes.Length; $i += 16) {
        $line = "    "
        $end = [Math]::Min($i + 16, $bytes.Length)
        for ($j = $i; $j -lt $end; $j++) {
            $line += "0x{0:X2}" -f $bytes[$j]
            if ($j -lt $bytes.Length - 1) {
                $line += ", "
            }
        }
        $content += "`n$line"
    }
    
    $content += @"

};

static const unsigned int ${VariableName}_size = $($bytes.Length);
"@
    
    [System.IO.File]::WriteAllText($OutputFile, $content)
}

# Generate C header from text data (GLSL) using raw string literals
function Generate-TextHeader {
    param(
        [string]$InputFile,
        [string]$OutputFile,
        [string]$VariableName
    )
    
    $text = [System.IO.File]::ReadAllText($InputFile)
    
    $content = @"
// Generated shader header for $VariableName
// Generated from: $(Split-Path $InputFile -Leaf)

#pragma once

static const char* ${VariableName}_source = R"GLSL($text)GLSL";
"@
    
    [System.IO.File]::WriteAllText($OutputFile, $content)
}

# Compile a single shader through all pipelines
function Compile-Shader {
    param(
        [hashtable]$Shader,
        [string]$Type,
        [string]$Profile,
        [string]$Suffix
    )
    
    Write-Host "  Compiling $($Shader.Name) ($Type shader)..." -ForegroundColor White
    
    # D3D11 compilation
    Write-Host "    - D3D11..." -NoNewline
    $d3d11Output = "$OutputDir/temp/$($Shader.Name)$Suffix.cso"
    Compile-D3D11 -InputFile $Shader.File -OutputFile $d3d11Output -Profile $Profile
    Generate-BinaryHeader -InputFile $d3d11Output `
                         -OutputFile "$OutputDir/headers/d3d11/$($Shader.Name)$Suffix.h" `
                         -VariableName "$($Shader.Name)$Suffix"
    Write-Host " Done" -ForegroundColor Green
    
    # SPIR-V compilation (temporary file)
    Write-Host "    - SPIR-V..." -NoNewline
    $spirvOutput = "$OutputDir/temp/$($Shader.Name).spv"
    $dxcProfile = $Profile -replace "_4_0", "_6_0"  # DXC minimum is SM 6.0
    Compile-SPIRV -InputFile $Shader.File -OutputFile $spirvOutput -Profile $dxcProfile
    Write-Host " Done" -ForegroundColor Green
    
    # GLSL cross-compilation
    Write-Host "    - GLSL..." -NoNewline
    $glslOutput = "$OutputDir/temp/$($Shader.Name).glsl"
    Compile-GLSL -InputFile $spirvOutput -OutputFile $glslOutput
    $glslSuffix = if ($Type -eq "Vertex") { "_vs" } else { "_fs" }
    Generate-TextHeader -InputFile $glslOutput `
                       -OutputFile "$OutputDir/headers/glsl/$($Shader.Name)$glslSuffix.h" `
                       -VariableName "$($Shader.Name)$glslSuffix"
    Write-Host " Done" -ForegroundColor Green
    
    # Metal cross-compilation (source files only - compiled on macOS)
    Write-Host "    - Metal..." -NoNewline
    $metalOutput = "$OutputDir/metal/$($Shader.Name).metal"
    Compile-Metal -InputFile $spirvOutput -OutputFile $metalOutput
    Write-Host " Done" -ForegroundColor Green
}

# Main compilation process
function Compile-AllShaders {
    Write-Host "`nCompiling shaders..." -ForegroundColor Cyan
    
    # Compile vertex shaders
    Write-Host "`nVertex Shaders:" -ForegroundColor Yellow
    foreach ($shader in $VertexShaders) {
        Compile-Shader -Shader $shader -Type "Vertex" -Profile "vs_4_0" -Suffix "_vs"
    }
    
    # Compile pixel shaders
    Write-Host "`nPixel Shaders:" -ForegroundColor Yellow
    foreach ($shader in $PixelShaders) {
        Compile-Shader -Shader $shader -Type "Pixel" -Profile "ps_4_0" -Suffix "_ps"
    }
}

# Generate platform-specific master headers
function Generate-MasterHeaders {
    Write-Host "`n=== Generating Master Headers ===" -ForegroundColor Cyan
    
    # D3D11 Master Header
    Write-Host "Generating D3D11 master header..." -ForegroundColor Yellow
    $d3d11Content = @"
// Generated D3D11 shader header - Windows platform
// Include this file to access all D3D11 compiled shaders

#pragma once

// Vertex Shaders
"@
    foreach ($shader in $VertexShaders) {
        $d3d11Content += "`n#include `"$($shader.Name)_vs.h`""
    }
    $d3d11Content += "`n`n// Pixel Shaders"
    foreach ($shader in $PixelShaders) {
        $d3d11Content += "`n#include `"$($shader.Name)_ps.h`""
    }
    [System.IO.File]::WriteAllText("$OutputDir/headers/d3d11/shaders.h", $d3d11Content)
    
    # GLSL Master Header  
    Write-Host "Generating GLSL master header..." -ForegroundColor Yellow
    $glslContent = @"
// Generated GLSL shader header - Linux/OpenGL platform
// Include this file to access all GLSL compiled shaders

#pragma once

// Vertex Shaders
"@
    foreach ($shader in $VertexShaders) {
        $glslContent += "`n#include `"$($shader.Name)_vs.h`""
    }
    $glslContent += "`n`n// Fragment Shaders"
    foreach ($shader in $PixelShaders) {
        $glslContent += "`n#include `"$($shader.Name)_fs.h`""
    }
    [System.IO.File]::WriteAllText("$OutputDir/headers/glsl/shaders.h", $glslContent)
    
    Write-Host "Master headers generated successfully" -ForegroundColor Green
}

# Generate platform-specific header lists (for documentation)
function Generate-HeaderInfo {
    Write-Host "`n=== Generated Files ===" -ForegroundColor Cyan
    Write-Host "D3D11 Headers (Windows):" -ForegroundColor Yellow
    Get-ChildItem "$OutputDir/headers/d3d11/*.h" | ForEach-Object { Write-Host "  $($_.Name)" }
    
    Write-Host "`nGLSL Headers (Linux):" -ForegroundColor Yellow  
    Get-ChildItem "$OutputDir/headers/glsl/*.h" | ForEach-Object { Write-Host "  $($_.Name)" }
    
    Write-Host "`nMetal Source Files (macOS):" -ForegroundColor Yellow
    Get-ChildItem "$OutputDir/metal/*.metal" | ForEach-Object { Write-Host "  $($_.Name)" }
    
    Write-Host "`nNote: Metal source files will be compiled to bytecode on macOS at build time" -ForegroundColor Gray
}

# Main execution
try {
    Write-Host "AppCore Shader Build System" -ForegroundColor Magenta
    Write-Host "===========================" -ForegroundColor Magenta
    
    if ($Clean) {
        Clean-Output
        exit 0
    }
    
    Verify-Tools
    Initialize-OutputDirectories
    Compile-AllShaders
    Generate-MasterHeaders
    Generate-HeaderInfo
    
    Write-Host "`nShader compilation completed successfully!" -ForegroundColor Green
    Write-Host "Output directory: $((Resolve-Path $OutputDir).Path)" -ForegroundColor Green
    Write-Host "`nCommit the following to the repository:" -ForegroundColor Cyan
    Write-Host "  headers/d3d11/  - D3D11 binary headers + shaders.h master header" -ForegroundColor Gray
    Write-Host "  headers/glsl/   - GLSL text headers + shaders.h master header" -ForegroundColor Gray  
    Write-Host "  metal/          - Metal source files" -ForegroundColor Gray
    Write-Host "`nGPU drivers can include platform-specific master headers:" -ForegroundColor Cyan
    Write-Host "  Windows: #include `"d3d11/shaders.h`"" -ForegroundColor Gray
    Write-Host "  Linux:   #include `"glsl/shaders.h`"" -ForegroundColor Gray
    Write-Host "  macOS:   #include `"shaders.h`" (generated at build time)" -ForegroundColor Gray
}
catch {
    Write-Host "`nError: $_" -ForegroundColor Red
    exit 1
}