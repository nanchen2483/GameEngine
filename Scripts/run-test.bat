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
  echo Usage: run-test.bat [debug/release]
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
  echo Usage: run-test.bat %configuration% [x64/x86]
  exit 1
)

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call vsenv.bat %architecture%

rem Run tests
pushd ..\out\build\%architecture%-%configuration%\Editor
call .\Test
if %ERRORLEVEL% GEQ 1 (
	exit 1
)
popd
popd
