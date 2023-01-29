@echo off
setlocal enabledelayedexpansion

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------




cd ..
set root_path=%cd%

if not exist "%root_path%\external\vcpkg\scripts\buildsystems\vcpkg.cmake" (
	echo INFO: You forgot to download the submodules. I'll fix that for you.
	git submodule update --init
)

if not exist "build/" (
	echo INFO: First time setup will take longer as the dependencies need to be downloaded and compiled.
) else (
	rmdir /S /Q "build"
)

set build_type=
set triplet_value=arm64-android
set feature_list=
set silent=

for %%A in (%*) do (
	if "%%A" == "-static" (
		set build_type=-DBUILD_SHARED_LIBS=OFF
		set triplet_value=x64-windows-static
	)
	if "%%A" == "-ui" (
		set "feature_list=!feature_list! -DELEMD_UI=ON"
	)
	if "%%A" == "-audio" (
		set "feature_list=!feature_list! -DELEMD_AUDIO=ON"
	)
	if "%%A" == "-video" (
		set "feature_list=!feature_list! -DELEMD_VIDEO=ON"
	)

	if "%%A" == "-s" (
		set "silent=true"
	)
)

cmake -B "build" -S . ^
	-G "MinGW Makefiles" ^
	-DVCPKG_TARGET_TRIPLET="%triplet_value%" ^
	-DVCPKG_OVERLAY_PORTS="%root_path%\external\custom-ports" ^
	-DCMAKE_TOOLCHAIN_FILE="%root_path%\external\vcpkg\scripts\buildsystems\vcpkg.cmake" %build_type% ^
  	-DANDROID_PLATFORM="android-21" ^
  	-DANDROID_ABI="arm64-v8a" ^
  	-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="%ANDROID_NDK_HOME%\build\cmake\android.toolchain.cmake"
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%silent%" == "true" pause
exit /b %err%