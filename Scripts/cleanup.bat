@echo off

set cleanAll=0
if "%1" == "--all" (
    set cleanAll=1
) else if "%1" == "-a" (
    set cleanAll=1
) else if "%1" == "" (
    rem Ignore
) else if "%1" == " " (
    rem Ignore
) else (
    echo unknown option: "%1"
    exit 1
)

rem Clean all untracked files except .vs
pushd %~dp0..
call git clean -dfx -e .vs
if %cleanAll% == 1 (
    pushd Dependencies\vcpkg
    call git clean -dfx
    popd
)
popd

if /i not "%~1"=="nopause" pause
