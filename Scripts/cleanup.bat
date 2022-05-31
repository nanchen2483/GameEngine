@echo off

rem Clean all untracked file except .vs
pushd %~dp0..\
call git clean -dfx -e .vs
popd

if /i not "%~1"=="nopause" pause
