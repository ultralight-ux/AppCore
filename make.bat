@echo off
SETLOCAL
set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
if "%1"=="release" GOTO CHECKTYPE
if "%1"=="debug" GOTO CHECKTYPE
if "%1"=="vs2015" GOTO CHECKTYPE
if "%1"=="vs2019" GOTO CHECKTYPE
GOTO SYNTAX
:CHECKTYPE
if "%2"=="x86" (
  echo Targeting the x86 platform ^(using the amd64_x86 toolchain^).
  set CFG=amd64_x86
  GOTO VALID
)
if "%2"=="x64" (
  echo Targeting the x64 platform.
  set CFG=amd64
  GOTO VALID
)
GOTO SYNTAX
:VALID
set BUILD_TESTS=0
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
  if "%1"=="vs2019" (
    echo D3D12 driver enabled.
    set USE_D3D12=1
  ) else (
    echo D3D12 driver only compatible with VS2019 build option, not enabling.
  )
)
if "%4"=="USE_D3D12" (
  if "%1"=="vs2019" (
    echo D3D12 driver enabled.
    set USE_D3D12=1
  ) else (
    echo D3D12 driver only compatible with VS2019 build option, not enabling.
  )
)
set "DIRNAME=build_%1_%2"
if "%1"=="vs2015" (
  if "%2"=="x86" (
    echo Generating projects for Visual Studio 2015.
    set "FLAGS=-G "Visual Studio 14 2015" -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
  ) else (
    echo Generating projects for Visual Studio 2015.
    set "FLAGS=-G "Visual Studio 14 2015 Win64" -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
  )
)
if "%1"=="vs2019" (
  if "%2"=="x86" (
    echo Generating projects for Visual Studio 2019.
    set "FLAGS=-G "Visual Studio 16 2019" -A Win32 -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
  ) else (
    echo Generating projects for Visual Studio 2019.
    set "FLAGS=-G "Visual Studio 16 2019" -A x64 -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
  )
)
if "%1"=="release" (
  echo Generating projects for Ninja ^(Minimum Size / Release^).
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
)
if "%1"=="debug" (
  echo Generating projects for Ninja ^(Release with Debug Info^).
  set "FLAGS=-G "Ninja" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_TESTS=%BUILD_TESTS% -DUSE_D3D12=%USE_D3D12%"
)
call "%VCVARS%" %CFG%
if not exist %DIRNAME% mkdir %DIRNAME%
cd %DIRNAME%
set CC=cl.exe
set CXX=cl.exe
echo %FLAGS%
cmake .. %FLAGS%
if "%1"=="vs2015" GOTO FINISH
if "%1"=="vs2019" GOTO FINISH
ninja
GOTO FINISH
:SYNTAX
echo.
echo usage: make [ release ^| debug ^| vs2015 ^| vs2019 ]  [ x64 ^| x86 ] [BUILD_TESTS] [USE_D3D12]
echo.
echo Build type parameter descriptions:
echo.
echo     release     Build minimum-size release via Ninja/VS2015
echo     debug       Build release with debug symbols via Ninja/VS2015
echo     vs2015      Generate VS2015 project files, does not perform a build.
echo     vs2019      Generate VS2019 project files, does not perform a build.
echo.
echo Configuration parameter descriptions:
echo.
echo     x64         Compile binaries for the x64 (amd64) platform.
echo     x86         Cross-compile binaries for the x86 (amd64_x86) platform.
echo.
echo Additional, optional build options:
echo.
echo     BUILD_TESTS  Whether tests should be built (currently MiniBrowser test shell). Disabled by default.
echo     USE_D3D12    Whether the D3D12 driver should be used instead of D3D11. Only compatible with vs2019. Disabled by default.
:FINISH
cd ..