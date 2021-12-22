@echo off
SETLOCAL
set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
if "%1"=="release" GOTO CHECKTYPE
if "%1"=="release_min" GOTO CHECKTYPE
if "%1"=="release_dbg" GOTO CHECKTYPE
if "%1"=="debug" GOTO CHECKTYPE
if "%1"=="vs" GOTO CHECKTYPE
GOTO SYNTAX
:CHECKTYPE
if "%2"=="x64" (
  set CFG=amd64
  GOTO VALID
)

GOTO SYNTAX
:VALID

set "DIRNAME=build_%1_%2"
set SKIP_BUILD=0

if "%1"=="vs" (
  set "FLAGS=-G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release"
  set SKIP_BUILD=1
)
if "%1"=="release" (
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=Release"
)
if "%1"=="release_min" (
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=MinSizeRel"
)
if "%1"=="release_dbg" (
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=RelWithDebInfo"
)
if "%1"=="debug" (
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=Debug"
)

set "FLAGS=%FLAGS% -DUWP_PLATFORM=0 -DWINDOWS_DESKTOP_PLATFORM=1"

set PROFILER=0
set USE_LOCAL_DEPS=0
set USE_D3D12=0
set D3D_DRIVER="d3d11"

:PROCESS_ARG
if not "%3" == "" (
  if "%3"=="local" (
    set USE_LOCAL_DEPS=1
  )
  if "%3"=="use_d3d12" (
    set USE_D3D12=1
  )
  if "%3"=="profiler" ( 
    set PROFILER=1
  )
  shift
  goto PROCESS_ARG
)

if %PROFILER%==1 (
  echo Enabling Tracy Profiler
  set "FLAGS=%FLAGS% -DUL_ENABLE_PROFILER=1"
  set "DIRNAME=%DIRNAME%_profiler"
)

if %USE_LOCAL_DEPS%==1 (
  echo Using local dependencies.
) else (
  echo Fetching dependencies from remote server.
)
set "FLAGS=%FLAGS% -DUSE_LOCAL_DEPS=%USE_LOCAL_DEPS%"

if %USE_D3D12%==1 (
  echo Using D3D12 backend.
  set D3D_DRIVER="d3d12"
) else (
  echo Using D3D11 backend.
)
set "FLAGS=%FLAGS% -DUL_D3D_DRIVER=%D3D_DRIVER%"

call "%VCVARS%" %CFG%
if not exist %DIRNAME% mkdir %DIRNAME%
cd %DIRNAME%
set CC=cl.exe
set CXX=cl.exe

cmake .. %FLAGS%
if %SKIP_BUILD%==1 (
  GOTO FINISH
)
ninja
GOTO FINISH
:SYNTAX
echo.
echo usage: make [ release ^| release_min ^| release_dbg ^| debug ^| vs ]  [ x64 ] [ local ] [ profiler ] [ use_d3d12 ]
echo.
echo Build type parameter descriptions:
echo.
echo     release      Build release optimized for performance via Ninja/VS2019
echo     release_min  Build release optimized for size via Ninja/VS2019
echo     release_dbg  Build release with debug symbols via Ninja/VS2019
echo     debug        Build debug with all checks and debug symbols via Ninja/VS2019
echo     vs           Generate VS2019 project files, does not perform a build.
echo.
echo Configuration parameter descriptions:
echo.
echo     x64         Compile binaries for the x64 (amd64) platform.
echo.
echo Additional, optional build options:
echo.
echo     local       Use local dependencies in deps folder (don't fetch from server).
echo     profiler    Whether or not to enable runtime profiling via Tracy. (Other modules must be built with profiler enabled.)
echo     use_d3d12   Whether the D3D12 driver should be used instead of D3D11. Only compatible with vs2019.
:FINISH
cd ..