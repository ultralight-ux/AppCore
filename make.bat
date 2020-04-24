@echo off
SETLOCAL
set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
if "%1"=="release" GOTO CHECKTYPE
if "%1"=="full_release" GOTO CHECKTYPE
if "%1"=="debug" GOTO CHECKTYPE
if "%1"=="full_debug" GOTO CHECKTYPE
if "%1"=="vs" GOTO CHECKTYPE
GOTO SYNTAX
:CHECKTYPE
if "%2"=="x64" (
  echo Targeting the x64 platform.
  set CFG=amd64
  GOTO VALID
)
if "%2"=="x64_uwp" (
  echo Targeting the x64 UWP platform.
  set CFG=amd64 uwp
  GOTO VALID
)
GOTO SYNTAX
:VALID
set BUILD_TESTS=1
set USE_D3D12=0
if "%3"=="BUILD_TESTS" (
  echo Tests enabled.
  set BUILD_TESTS=1
)
if "%4"=="BUILD_TESTS" (
  echo Tests enabled.
  set BUILD_TESTS=1
)
if "%3"=="USE_D3D12" (
  echo D3D12 driver enabled.
  set USE_D3D12=1
)
if "%4"=="USE_D3D12" (
  echo D3D12 driver enabled.
  set USE_D3D12=1
)
set "DIRNAME=build_%1_%2"
if "%1"=="vs" (
  echo Generating projects for Visual Studio 2019.
  set "FLAGS=-G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
)
if "%1"=="release" (
  echo Generating projects for Ninja ^(Minimum Size / Release^).
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
)
if "%1"=="full_release" (
  echo Generating projects for Ninja ^(Max Speed / Release^).
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
)
if "%1"=="debug" (
  echo Generating projects for Ninja ^(Release with Debug Info^).
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
)
if "%1"=="full_debug" (
  echo Generating projects for Ninja ^(Debug^).
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
)
call "%VCVARS%" %CFG%
if not exist %DIRNAME% mkdir %DIRNAME%
cd %DIRNAME%
set CC=cl.exe
set CXX=cl.exe
set "EXTRA_FLAGS=-DUWP_PLATFORM=0"
if "%2"=="x64_uwp" (
  echo Using UWP Platform.
  set "EXTRA_FLAGS=-DUWP_PLATFORM=1 -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0"
)
cmake .. %FLAGS% %EXTRA_FLAGS%
if "%1"=="vs" GOTO FINISH
ninja
GOTO FINISH
:SYNTAX
echo.
echo usage: make [ release ^| debug ^| full_release ^| full_debug ^| vs  ]   [ x64 ^| x64_uwp ]  [BUILD_TESTS] [USE_D3D12]
echo.
echo Build type parameter descriptions:
echo.
echo     release       Build minimum-size release via Ninja/VS2019
echo     debug         Build release with debug symbols via Ninja/VS2019
echo     full_release  Build fully-optimized release via Ninja/VS2019
echo     full_debug    Build unoptimized debug via Ninja/VS2019
echo     vs            Generate VS2019 project files, does not perform a build.
echo.
echo Configuration parameter descriptions:
echo.
echo     x64         Compile binaries for the x64 (amd64) platform.
echo     x64_uwp     Compile binaries for the x64 (amd64) UWP (Universal Windows Platform) platform.
echo.
echo Additional, optional build options:
echo.
echo     BUILD_TESTS  Whether tests should be built (currently MiniBrowser test shell). Enabled by default.
echo     USE_D3D12    Whether the D3D12 driver should be used instead of D3D11. Only compatible with vs2019. Disabled by default.
:FINISH
cd ..