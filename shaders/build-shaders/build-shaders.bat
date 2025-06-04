@echo off
REM build-shaders.bat
REM Windows batch wrapper for build-shaders.ps1

setlocal enabledelayedexpansion

REM Check for PowerShell
where powershell >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: PowerShell not found in PATH
    echo Please ensure PowerShell is installed and accessible
    exit /b 1
)

REM Parse command line arguments
set "PS_ARGS="
set "HELP=0"

:parse_args
if "%~1"=="" goto :done_parsing
if /i "%~1"=="-help" set "HELP=1" & goto :next_arg
if /i "%~1"=="--help" set "HELP=1" & goto :next_arg
if /i "%~1"=="/?" set "HELP=1" & goto :next_arg
if /i "%~1"=="-debug" set "PS_ARGS=!PS_ARGS! -Debug" & goto :next_arg
if /i "%~1"=="-clean" set "PS_ARGS=!PS_ARGS! -Clean" & goto :next_arg
if /i "%~1"=="-outputdir" (
    set "PS_ARGS=!PS_ARGS! -OutputDir '%~2'"
    shift
    goto :next_arg
)
if /i "%~1"=="-toolsdir" (
    set "PS_ARGS=!PS_ARGS! -ToolsDir '%~2'"
    shift
    goto :next_arg
)

REM Unknown argument
echo Warning: Unknown argument '%~1'

:next_arg
shift
goto :parse_args

:done_parsing

if "%HELP%"=="1" (
    echo AppCore Shader Build System
    echo ===========================
    echo.
    echo Usage: build-shaders.bat [OPTIONS]
    echo.
    echo Options:
    echo   -debug          Enable shader debugging ^(include debug symbols^)
    echo   -clean          Clean output directory and exit
    echo   -outputdir DIR  Specify output directory ^(default: ../generated^)
    echo   -toolsdir DIR   Specify tools directory ^(default: ./tools^)
    echo   -help           Show this help message
    echo.
    echo Examples:
    echo   build-shaders.bat                    ^(Basic compilation^)
    echo   build-shaders.bat -debug             ^(Debug build^)
    echo   build-shaders.bat -clean             ^(Clean output^)
    echo   build-shaders.bat -outputdir C:\Out  ^(Custom output directory^)
    echo.
    exit /b 0
)

REM Get script directory
set "SCRIPT_DIR=%~dp0"

REM Run PowerShell script
echo Running AppCore shader build system...
powershell -NoProfile -ExecutionPolicy Bypass -File "%SCRIPT_DIR%build-shaders.ps1" %PS_ARGS%

REM Check exit code
if %errorlevel% neq 0 (
    echo.
    echo Build failed with exit code %errorlevel%
    exit /b %errorlevel%
)

echo.
echo Build completed successfully!
exit /b 0