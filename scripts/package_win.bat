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
	
    :: Calculator
	xcopy ".\build\examples\calculator\res" ".\samples\calculator\res" /I/E
	xcopy ".\build\examples\calculator\Release\calculator.exe" ".\samples\calculator\"

	:: Slack
	xcopy ".\build\examples\slack\res" ".\samples\slack\res" /I/E
	xcopy ".\build\examples\slack\Release\slack.exe" ".\samples\slack\"

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