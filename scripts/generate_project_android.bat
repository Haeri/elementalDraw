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
) else (
	rmdir /S /Q "build"
)

mkdir "build"
cd build

set build_type=
set triplet_value=arm64-android
if "%1" == "-static" (
	set build_type=-DBUILD_SHARED_LIBS=OFF
	set triplet_value=x64-windows-static
)

cmake .. ^
	-G "MinGW Makefiles" ^
	-DVCPKG_TARGET_TRIPLET="%triplet_value%" ^
	-DVCPKG_OVERLAY_PORTS="%root_path%\external\custom-ports" ^
	-DCMAKE_TOOLCHAIN_FILE="%root_path%/external/vcpkg/scripts/buildsystems/vcpkg.cmake" %build_type% ^
  	-DANDROID_PLATFORM="android-21" ^
  	-DANDROID_ABI="arm64-v8a" ^
  	-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="%ANDROID_NDK_HOME%/build/cmake/android.toolchain.cmake"
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%