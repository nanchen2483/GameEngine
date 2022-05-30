@echo off

for /f "usebackq delims=#" %%a in (`"%programfiles(x86)%\Microsoft Visual Studio\Installer\vswhere" -property catalog_productLineVersion`) do (
	if %%a == 2022 ( goto :EOF )
)
echo 'Visual Studio 2022' not found
exit 1
