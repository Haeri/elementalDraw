#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")
script_path=$(pwd)

cd ../data/shader
for f in *.vert *.frag
	do $script_path"/../external/vcpkg/installed/x64-osx/tools/glslangValidator" -V $f  -o $f.spv
done

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $old_path
