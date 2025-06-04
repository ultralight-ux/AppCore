# AppCore Shader Build System

This directory contains a Windows-only offline shader build system for AppCore. It compiles HLSL shaders to multiple target formats (D3D11, SPIR-V, GLSL, Metal) and generates C header files for embedding in the application.

## Directory Structure

```
build-shaders/
├── CMakeLists.txt              # CMake build configuration
├── build-shaders.ps1           # PowerShell build script
├── GenerateBinaryHeader.cmake  # Binary to C header conversion
├── GenerateTextHeader.cmake    # Text to C header conversion
├── GenerateMasterHeader.cmake  # Master header generation
├── tools/                      # Tools directory (not included)
│   ├── dxc/                    # DirectX Shader Compiler
│   └── spirv-cross/            # SPIRV-Cross tool
└── README.md                   # This file

../generated/                   # Output directory
├── temp/                       # Temporary files (ignored by git)
├── metal/                      # Metal source files (.metal) - for macOS
└── headers/                    # Generated C headers (committed to repo)
    ├── d3d11/                  # D3D11 binary headers (Windows)
    └── glsl/                   # GLSL text headers (Linux)
```

## Prerequisites

### Required Tools

1. **FXC**
   - Included in repository at `../tools/fxc.exe`
   - Required for D3D11 shader compilation (Shader Model 4.0)

2. **DirectX Shader Compiler (DXC)**
   - Automatically downloaded from GitHub releases on first run
   - Latest version (v1.8.2505) with SPIR-V support
   - Required for SPIR-V generation (Shader Model 5.0)

3. **SPIRV-Cross**
   - Included in repository at `../tools/spirv-cross.exe`
   - Required for GLSL and Metal cross-compilation

### Tool Setup

The tools are already included in the repository:

```
../tools/
├── fxc.exe                     # Included in repo
├── spirv-cross.exe             # Included in repo
└── dxc/                        # Downloaded automatically on first run
    └── bin/
        └── x64/
            └── dxc.exe
```

No manual setup required - the build script handles everything automatically.

## Usage

### PowerShell Script (Recommended)

```powershell
# Basic compilation
.\build-shaders.ps1

# Debug build (with debug info)
.\build-shaders.ps1 -Debug

# Clean output directory
.\build-shaders.ps1 -Clean

# Custom output directory
.\build-shaders.ps1 -OutputDir "C:\MyShaders"

# Custom tools directory
.\build-shaders.ps1 -ToolsDir "C:\MyTools"
```

### CMake Build

```bash
# Configure
mkdir build
cd build
cmake ..

# Build
cmake --build . --target compile_shaders

# With debug info
cmake -DSHADER_DEBUG=ON ..
cmake --build . --target compile_shaders
```

## Output Files

The build system generates the following committed files:

### D3D11 Headers (Windows)
```cpp
#include "d3d11/shaders.h"           // Master header - includes all D3D11 shaders
// Individual headers:
// vertex_path_vs_data, vertex_path_vs_size
// fill_ps_data, fill_ps_size
// etc.
```

### GLSL Headers (Linux/OpenGL)
```cpp
#include "glsl/shaders.h"            // Master header - includes all GLSL shaders
// Individual headers contain raw string literals:
// vertex_path_vs_source (R"GLSL(...)GLSL")
// fill_fs_source (R"GLSL(...)GLSL") (note: _fs not _ps)
// etc.
```

### Metal Source Files (macOS)
```
metal/vertex_path.metal              // Metal source compiled on macOS at build time
metal/fill.metal                     // Metal source compiled on macOS at build time
```

### Metal Headers (Generated on macOS at build time)
```cpp
#include "shaders.h"                 // Master header - includes all Metal shaders
// Individual headers:
// vertex_path_vs_data, vertex_path_vs_size (metallib bytecode)
// fill_ps_data, fill_ps_size (metallib bytecode)
// etc.
```

## Compilation Pipeline

The build system uses the following compilation flow:

### Windows (Offline Build)
```
HLSL Source Files
    ↓
    ├── FXC → D3D11 Bytecode → Binary Header (Windows)
    └── DXC → SPIR-V
                ↓
                ├── SPIRV-Cross → GLSL → Text Header (Linux)
                └── SPIRV-Cross → Metal Source → Text Header
```

### macOS (Build-Time Compilation)
```
Generated Metal Source (.metal)
    ↓
    xcrun metal → AIR → metallib → Binary Header (macOS)
```

**Note**: Metal shaders require a two-step process:
1. Windows generates Metal source code (.metal files)
2. macOS compiles Metal source to metallib bytecode at build time

### Shader Profiles

- **D3D11**: Shader Model 4.0 (`vs_4_0`, `ps_4_0`) for maximum compatibility
- **SPIR-V**: Shader Model 6.0 (`vs_6_0`, `ps_6_0`) via DXC (minimum supported)
- **GLSL**: Version 330 core profile (desktop OpenGL)
- **Metal**: Version 2.1 with argument buffers

## Integration with AppCore

### Step 1: Generate Shaders (Windows)
1. Run the Windows build system to generate cross-platform shader files:
   ```bash
   cd shaders/build-shaders
   .\build-shaders.bat
   ```
2. Commit the generated files in `shaders/generated/` to the repository

### Step 2: Build AppCore
- **Windows/Linux**: CMake automatically uses pre-generated headers
- **macOS**: CMake compiles Metal source files to metallib at build time

### Usage in Code
```cpp
// Include platform-specific master headers
#ifdef _WIN32
    #include "d3d11/shaders.h"
    // D3D11 - binary data
    device->CreateVertexShader(vertex_path_vs_data, vertex_path_vs_size, nullptr, &vs);
    device->CreatePixelShader(fill_ps_data, fill_ps_size, nullptr, &ps);
#elif defined(__linux__)
    #include "glsl/shaders.h"
    // OpenGL - GLSL source
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertex_path_vs_source);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fill_fs_source);
#elif defined(__APPLE__)
    #include "shaders.h"  // Located in build output directory
    // Metal - metallib binary data (compiled at build time)
    NSData* vsData = [NSData dataWithBytes:vertex_path_vs_data length:vertex_path_vs_size];
    NSData* psData = [NSData dataWithBytes:fill_ps_data length:fill_ps_size];
    id<MTLLibrary> vsLibrary = [device newLibraryWithData:vsData error:nil];
    id<MTLLibrary> psLibrary = [device newLibraryWithData:psData error:nil];
#endif
```

## Troubleshooting

### Compilation Errors
- Check HLSL syntax in source files
- Ensure `common.hlsli` and `clip.hlsli` are present
- Verify shader entry points are named `main`

### macOS Metal Compilation Issues
```
Warning: Metal source directory not found
```
- Run the Windows build system first to generate Metal source files
- Ensure `shaders/generated/metal/` directory exists with `.metal` files

```
Error: xcrun: command not found
```
- Install Xcode command line tools: `xcode-select --install`
- Verify Metal compiler: `xcrun -sdk macosx metal --version`

## Development Notes

- The build system is Windows-only but generates cross-platform output
- HLSL shaders must use `main` as the entry point
- Include files (`common.hlsli`, `clip.hlsli`) are automatically tracked as dependencies
- Debug builds include symbols and disable optimizations