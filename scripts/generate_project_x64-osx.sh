#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
root_path=$(pwd)

triplet_value="x64-osx-dynamic"
lib_linkage=""
feature_list=""

if [ ! -d $root_path"/external/vcpkg/scripts/buildsystems/" ]; then
	echo "INFO: You forgot to download the submodules. I'll fix that for you."
	git submodule update --init
fi

for var in "$@"
do
	if [ "$var" = "-static" ]; then
		triplet_value="x64-osx"
		lib_linkage="-DBUILD_SHARED_LIBS=OFF"
	fi
	if [ "$var" = "-ui" ]; then
		feature_list="-DELEMD_UI=ON $feature_list"
	fi
	if [ "$var" = "-audio" ]; then
		feature_list="-DELEMD_AUDIO=ON $feature_list"
	fi
	if [ "$var" = "-video" ]; then
		feature_list="-DELEMD_VIDEO=ON $feature_list"
	fi
done

if [ ! -d "build/" ]; then
	echo "INFO: First time setup will take longer as the dependencies need to be downloaded and compiled."
else
	rm -rf build
fi

cmake -B "build" -S . -G Xcode \
	-DVCPKG_TARGET_TRIPLET=$triplet_value \
	-DVCPKG_OVERLAY_PORTS=$root_path"/external/custom-ports" \
	-DCMAKE_TOOLCHAIN_FILE=$root_path"/external/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	$lib_linkage \
	$feature_list
err=$?

if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path