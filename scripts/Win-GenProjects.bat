@echo off
pushd ..\
call util\bin\premake\premake5.exe vs2019
popd
PAUSE