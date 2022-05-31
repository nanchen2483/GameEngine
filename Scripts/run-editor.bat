@echo off

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call vsenv.bat

rem Run editor
pushd .\..\out\build\x64-debug
call ninja -j7 xEditor

pushd xEditor
call .\xEditor
popd
popd
popd
