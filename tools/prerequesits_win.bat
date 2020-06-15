@echo off

set oldpath=%cd% 
cd "%~dp0"

python ./prerequesits.py

if errorlevel 1 (
   exit /b %errorlevel%
)

cd "%oldpath%"
if not "%1" == "-s" pause