@echo off
pushd %~dp0..\out\build\x64-debug
call ninja -j7 EngineTest
pushd Engine.Test
call .\EngineTest
popd
popd
