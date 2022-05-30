@echo off

pushd %~dp0
call findvs.bat
if %ERRORLEVEL% GEQ 1 (
	exist 1
)
popd

pushd %~dp0..\Dependencies
if not exist "vcpkg\bootstrap-vcpkg.bat" (
	call git submodule update --init --recursive
)
if not exist "vcpkg\vcpkg.exe" (
	call .\vcpkg\bootstrap-vcpkg.bat
)
popd
