@echo off

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call vsenv.bat 64

rem Run tests
pushd ..\out\build\x64-debug\Test
call .\Test
if %ERRORLEVEL% GEQ 1 (
	exit 1
)
popd
popd
