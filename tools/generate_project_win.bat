@echo off

set oldpath=%cd% 
cd "%~dp0"

cd ..
rmdir /S /Q "build"
mkdir "build"
cd build

cmake .. 

cd "%oldpath%"
if not "%1" == "-s" pause
exit /b %errorlevel%