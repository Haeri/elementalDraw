@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------



cd ..
if "%1" == "-static" (
	xcopy ".\build\Release\elemd.lib" ".\lib\elemd-static.lib*"

	:: Roots of Darknes
	xcopy ".\examples\roots_of_darknes\res" ".\samples\roots_of_darknes\res" /I/E
	xcopy ".\build\examples\roots_of_darknes\Release\roots-of-darknes.exe" ".\samples\roots_of_darknes\"
	
) else (
	xcopy ".\build\Release" ".\bin" /I/E	
)

set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%