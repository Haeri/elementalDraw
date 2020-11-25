#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
root_path=$(pwd)
rm -rf build
mkdir build
cd build


cmake .. -DVCPKG_TARGET_TRIPLET=x64-osx -DVCPKG_OVERLAY_PORTS=$root_path"/external/custom-ports" -DVCPKG_VERBOSE=1 -DVCPKG_DISABLE_METRICS
err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $old_path