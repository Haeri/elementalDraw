#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

"$VCPKG_INSTALLATION_ROOT/installed/x64-linux/tools/glslangValidator.exe" -v

cd $oldpath