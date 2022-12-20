@echo off

pushd %~dp0
call cleanup.bat nopause
call build.bat
popd
