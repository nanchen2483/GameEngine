@echo off

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call setup.bat
call vsenv.bat

rem Check if 'Ninja' is installed
where ninja
if %ERRORLEVEL% neq 0 (echo 'Ninja' not found exit 1)

rem Check if 'cl.exe' exists
where cl.exe
if %ERRORLEVEL% neq 0 (echo 'cl.exe' not found exit 1)

rem Build with CMake
call cmake -S .\..\ --preset=x64-debug
popd
