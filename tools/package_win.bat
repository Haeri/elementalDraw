@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------



cd ..
if "%1" == "-static" (
	xcopy ".\build\Release\elemd.lib" ".\lib\elemd-static.lib*"

	:: ------ Package Examples ------

	:: Hello World
	xcopy ".\build\examples\hello_world\res" ".\samples\hello_world\res"
	xcopy ".\build\examples\hello_world\Release\hello-world.exe" ".\samples\hello_world\"
	::xcopy ".\build\examples\hello_world\Release\elemd.dll" ".\samples\hello_world\"

	:: Game Of Life
	xcopy ".\build\examples\game_of_life\res" ".\samples\game_of_life\res"
	xcopy ".\build\examples\game_of_life\Release\game-of-life.exe" ".\samples\game_of_life\"
	::xcopy ".\build\examples\game_of_life\Release\elemd.dll" ".\samples\game_of_life\"
) else (
	xcopy ".\build\Release\elemd.dll" ".\bin\"
	xcopy ".\build\Release\elemd.lib" ".\lib\"
)

set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%