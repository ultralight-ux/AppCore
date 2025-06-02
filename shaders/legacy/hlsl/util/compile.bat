@echo off
setlocal EnableDelayedExpansion

:: Set up environment
set "SHADER_SRC_PS=..\ps"
set "SHADER_SRC_VS=..\vs"
set "SHADER_BIN=..\bin"
set "PATH=%PATH%;%CD%"

:: Create array of PS shaders (actual file should end in .hlsl)
set "PS_SHADERS[0]=fill"
set "PS_SHADERS[1]=fill_path"
set "PS_SHADERS[2]=filter_basic"
set "PS_SHADERS[3]=filter_blur"

:: Create array of VS shaders (actual file should end in .hlsl)
set "VS_SHADERS[0]=v2f_c4f_t2f"
set "VS_SHADERS[1]=v2f_c4f_t2f_t2f_d28f"

echo Starting shader compilation...
echo -------------------------------------

:: Save current directory and change to binary directory
pushd "%SHADER_BIN%" 2>nul || (
    echo Error: Cannot access directory %SHADER_BIN%
    exit /b 1
)

:: Clean up existing header files
del /f /q *.h 2>nul

:: Compile pixel shaders
for %%i in (0,1,2,3) do (
    echo.
    echo [Pixel Shader] Compiling !PS_SHADERS[%%i]!...
    echo -------------------------------------
    fxc /T ps_4_0 /Fo !PS_SHADERS[%%i]!.fxc "%SHADER_SRC_PS%\!PS_SHADERS[%%i]!.hlsl" /E PS
    if !ERRORLEVEL! neq 0 (
        echo Error: Failed to compile !PS_SHADERS[%%i]!
        popd
        exit /b 1
    )
    echo -------------------------------------
)

:: Compile vertex shaders
for %%i in (0,1) do (
    echo.
    echo [Vertex Shader] Compiling !VS_SHADERS[%%i]!...
    echo -------------------------------------
    fxc /T vs_4_0 /Fo !VS_SHADERS[%%i]!.fxc "%SHADER_SRC_VS%\!VS_SHADERS[%%i]!.hlsl" /E VS
    if !ERRORLEVEL! neq 0 (
        echo Error: Failed to compile !VS_SHADERS[%%i]!
        popd
        exit /b 1
    )
    echo -------------------------------------
)

:: Generate headers
echo.
echo Generating headers...
echo -------------------------------------
for %%f in (*.fxc) do (
    echo Processing %%f...
    bin2header "%%f"
    if !ERRORLEVEL! neq 0 (
        echo Error: Failed to generate header for %%f
        popd
        exit /b 1
    )
)

:: Clean up intermediate files
del /f /q *.fxc 2>nul

:: Restore original directory
popd

echo.
echo Shader compilation completed successfully.
echo -------------------------------------
exit /b 0