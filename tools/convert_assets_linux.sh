#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

cd ../res/shader
for f in *.vert *.frag
	do "$VCPKG_INSTALLATION_ROOT/installed/x64-linux/tools/glslangValidator" -V $f  -o $f.spv
done

cd $oldpath