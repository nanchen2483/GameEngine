@echo off

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call vsenv.bat 64

rem Run editor
pushd ..\out\build\x64-debug\Editor
call .\Editor
popd
popd
