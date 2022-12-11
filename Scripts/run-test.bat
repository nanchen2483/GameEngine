@echo off

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call vsenv.bat 64

rem Run tests
pushd ..\out\build\x64-debug
call ninja -j7 Test

pushd Test
call .\Test
if %ERRORLEVEL% GEQ 1 (
	exit 1
)
popd
popd
popd
