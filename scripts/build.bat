@echo off
where ninja
if %ERRORLEVEL% neq 0 (
	echo 'Ninja' not found 
) else (
	cmake -S %~dp0..\ --preset=x64-debug
)
