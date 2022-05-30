@echo off

pushd %~dp0
call setup.bat
call vsenv.bat

where ninja
if %ERRORLEVEL% neq 0 (echo 'Ninja' not found exit 1)

where cl.exe
if %ERRORLEVEL% neq 0 (echo 'cl.exe' not found exit 1)

call cmake -S .\..\ --preset=x64-debug
popd
