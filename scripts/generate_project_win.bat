@echo off

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
set triplet_value=x64-windows
set feature_list=
set silent=

for %%A in (%*) do (
	if "%%A" == "-static" (
		set build_type=-DBUILD_SHARED_LIBS=OFF
		set triplet_value=x64-windows-static
	)
	if "%%A" == "-audio" (
		set feature_list="-DELEMD_AUDIO=ON %feature_list%"
	)
	if "%%A" == "-video" (
		set feature_list="-DELEMD_VIDEO=ON %feature_list%"
	)

	if "%%A" == "-s" (
		set "silent=true"
	)
)

cmake -B "build" -S . ^
	-DVCPKG_TARGET_TRIPLET="%triplet_value%" ^
	-DVCPKG_OVERLAY_PORTS="%root_path%\external\custom-ports" ^
	-DCMAKE_TOOLCHAIN_FILE="%root_path%\external\vcpkg\scripts\buildsystems\vcpkg.cmake" ^
	%build_type% ^
	%feature_list% 	
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%silent%" == "true" pause
exit /b %err%