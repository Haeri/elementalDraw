@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------





if "%1" == "-static"(
	mkdir lib
	cp ./build/Release/elemd.lib ./lib/elemd-static.lib
)else(
	mkdir bin
	mkdir lib
	cp ./build/Release/elemd.dll ./bin/
	cp ./build/Release/elemd.lib ./lib/
)
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%