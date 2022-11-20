@echo off

rem Check if Visual Studio is installed
pushd %~dp0
call findvs.bat
if %ERRORLEVEL% GEQ 1 (
	exist 1
)

rem Update submodule and setup
pushd ..\Dependencies
if not exist "vcpkg\bootstrap-vcpkg.bat" (
	call git submodule update --init --recursive
)

pushd vcpkg
if not exist "vcpkg.exe" (
	call bootstrap-vcpkg.bat
	call vcpkg integrate install
)
popd
popd
popd

