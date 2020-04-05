@echo off

cd ..
cd ElementalDraw
rmdir /S /Q "build"
mkdir "build"
cd build
cmake .. "-DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE=C:\Compiler\vcpkg\scripts\buildsystems\vcpkg.cmake"

pause