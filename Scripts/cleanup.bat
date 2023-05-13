@echo off

set cleanAll=0

if [%1] equ [--all] (
    set cleanAll=1
) else if [%1] equ [-a] (
    set cleanAll=1
) else if [%1] equ [] (
    rem Ignore
) else (
	echo Invalid arguments: "%1"
	echo Usage: cleanup.bat [--all/-a]
    exit 1
)

rem Clean all untracked files except .vs
pushd %~dp0..
call git clean -dfx -e .vs
if %cleanAll% equ 1 (
    pushd Dependencies\vcpkg
    call git clean -dfx
    popd
)
popd

if /i not "%~1"=="nopause" pause
