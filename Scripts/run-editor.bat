@echo off

pushd %~dp0
call vsenv.bat
popd

pushd %~dp0..\out\build\x64-debug
call ninja -j7 xEditor
pushd xEditor
call .\xEditor
popd
popd
