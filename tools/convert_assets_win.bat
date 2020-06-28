@echo off

set oldpath=%cd% 
cd "%~dp0"

"%VCPKG_INSTALLATION_ROOT%/installed/x64-windows/tools/glslangValidator.exe" -v

cd "%oldpath%"
if not "%1" == "-s" pause
exit /b %errorlevel%