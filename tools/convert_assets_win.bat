@echo off

set oldpath=%cd% 
cd "%~dp0"

cd ../res/shader
for %%i in (*.vert *.frag) do "%VCPKG_INSTALLATION_ROOT%/installed/x64-windows/tools/glslangValidator.exe" -V "%%~i" -o "%%~i.spv"

cd "%oldpath%"
if not "%1" == "-s" pause
exit /b %errorlevel%