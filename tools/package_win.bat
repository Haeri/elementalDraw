@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------



cd ..
if "%1" == "-static" (
	xcopy ".\build\Release\elemd.lib" ".\lib\elemd-static.lib*"
) else (
	xcopy ".\build\Release\elemd.dll" ".\bin\"
	xcopy ".\build\Release\elemd.lib" ".\lib\"
)

xcopy ".\build\examples\hello_world\Release\*" ".\samples\hello_world\"
xcopy ".\build\examples\hello_world\res\*" ".\samples\hello_world\res\"

set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%