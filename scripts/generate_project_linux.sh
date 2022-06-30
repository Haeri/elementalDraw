#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
root_path=$(pwd)

if [ ! -d $root_path"/external/vcpkg/scripts/buildsystems/vcpkg.cmake" ]; then
	echo "INFO: You forgot to download the submodules. I'll fix that for you."
	git submodule update --init
fi

if [ ! -d "build/" ]; then
	echo "INFO: First time setup will take longer as the dependencies need to be downloaded and compiled."
else
	rm -rf build
fi

build_type=""
feature_list=""

for var in "$@"
do
	if [ "$var" = "-static" ]; then
		build_type="-DBUILD_SHARED_LIBS=OFF"
	fi
	if [ "$var" = "-audio" ]; then
		feature_list="-DELEMD_AUDIO=ON $feature_list"
	)
	if [ "$var" = "-video" ]; then
		feature_list="-DELEMD_VIDEO=ON $feature_list"
	)
done

cmake -B "build" -S . -DVCPKG_TARGET_TRIPLET=x64-linux -DVCPKG_OVERLAY_PORTS=$root_path"/external/custom-ports" -DCMAKE_TOOLCHAIN_FILE=$root_path"/external/vcpkg/scripts/buildsystems/vcpkg.cmake" $build_type $feature_list
err=$?

if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path