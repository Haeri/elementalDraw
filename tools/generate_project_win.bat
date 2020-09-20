@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------




cd ..
rmdir /S /Q "build"
mkdir "build"
cd build

if "%VCPKG_ROOT%"=="" (
	cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows
)else (
	cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%"/scripts/buildsystems/vcpkg.cmake"
)

set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" pause
exit /b %err%