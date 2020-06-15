@echo off

set oldpath=%cd% 
cd "%~dp0"

cd ..
rmdir /S /Q "build"
mkdir "build"
cd build

cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="%VCPKG_INSTALLATION_ROOT%\scripts\buildsystems\vcpkg.cmake"

if errorlevel 1 (
   exit /b %errorlevel%
)

cd "%oldpath%"
if not "%1" == "-s" pause