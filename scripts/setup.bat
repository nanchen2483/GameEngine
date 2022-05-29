@echo off
pushd %~dp0..\Dependencies\vcpkg
call .\bootstrap-vcpkg.bat
popd