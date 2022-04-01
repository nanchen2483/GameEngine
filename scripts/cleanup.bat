@echo off
pushd ..\
call git clean -dfx -e .vs
popd
PAUSE
