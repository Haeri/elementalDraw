@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------




cd ..
set root_path=%cd%

if not exist "build/" (
	echo INFO: First time setup will take longer as the dependencies need to be downloaded and compiled.
	mkdir "build"
)
cd build

set build_type=
if "%1" == "-static" set build_type=-DBUILD_SHARED_LIBS=OFF

cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows-static -DVCPKG_OVERLAY_PORTS="%root_path%\external\custom-ports" -DCMAKE_TOOLCHAIN_FILE="%root_path%/external/vcpkg/scripts/buildsystems/vcpkg.cmake" %build_type%
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%