@echo off

rem Set the necessary environment variables to use the Visual Studio tools
pushd %~dp0
call vsenv.bat

rem Run tests
pushd .\..\out\build\x64-debug
call ninja -j7 EngineTest

pushd Engine.Test
call .\EngineTest
popd
popd
popd
