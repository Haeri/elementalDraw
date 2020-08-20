@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------




set scriptpath=%cd% 
cd ../res/shader
for %%i in (*.vert *.frag) do "%scriptpath%/../extern/vcpkg/installed/x64-windows/tools/glslangValidator.exe" -V "%%~i" -o "%%~i.spv"
set /a "err=%err%+%errorlevel%"



::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" pause
exit /b %err%