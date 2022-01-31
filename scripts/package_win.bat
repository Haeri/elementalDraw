@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------



cd ..
if "%1" == "-static" (
	xcopy ".\build\Release\elemd.lib" ".\lib\elemd-static.lib*"

	:: Hello World
	xcopy ".\build\examples\hello_world\res" ".\samples\hello_world\res" /I/E
	xcopy ".\build\examples\hello_world\Release\hello-world.exe" ".\samples\hello_world\"

	:: Overview
	xcopy ".\build\examples\overview\res" ".\samples\overview\res" /I/E
	xcopy ".\build\examples\overview\Release\overview.exe" ".\samples\overview\"

	:: Game Of Life
	xcopy ".\build\examples\game_of_life\Release\game-of-life.exe" ".\samples\game_of_life\"

	:: Platformer
	xcopy ".\build\examples\platformer\res" ".\samples\platformer\res" /I/E
	xcopy ".\build\examples\platformer\Release\platformer.exe" ".\samples\platformer\"

	:: Benchmark
	xcopy ".\build\examples\benchmark\Release\benchmark.exe" ".\samples\benchmark\"

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