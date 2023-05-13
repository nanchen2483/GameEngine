@echo off

set architecture=x64
set configuration=debug

if [%1] equ [debug] (
  set configuration=debug
) else if [%1] equ [release] (
  set configuration=release
) else if [%1] equ [] (
    rem Ignore
) else (
  echo Invalid arguments: "%1"
  echo Usage: build.bat [debug/release]
  exit 1
)

if [%2] equ [x64] (
  set architecture=x64
) else if [%2] equ [x86] (
  set architecture=x86
) else if [%2] equ [] (
    rem Ignore
) else (
  echo Invalid arguments: "%2"
  echo Usage: build.bat %configuration% [x64/x86]
  exit 1
)

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call setup.bat
call vsenv.bat %architecture%

rem Check if 'Ninja' is installed
where ninja
if %ERRORLEVEL% neq 0 (echo 'Ninja' not found exit 1)

rem Check if 'cl.exe' exists
where cl.exe
if %ERRORLEVEL% neq 0 (echo 'cl.exe' not found exit 1)

pushd ..
rem Generate project files with CMake
call cmake -S . --preset=%architecture%-%configuration%

rem Build with CMake
call cmake --build --preset=%architecture%-%configuration%
popd
popd
