#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
rm -rf build
mkdir build
cd build

if [[ -z "${VCPKG_ROOT}" ]]; then
	cmake .. -DVCPKG_TARGET_TRIPLET=x64-linux
else
	cmake .. -DVCPKG_TARGET_TRIPLET=x64-linux -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT"/scripts/buildsystems/vcpkg.cmake"
fi

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $old_path