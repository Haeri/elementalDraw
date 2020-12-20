#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
if [ "$1" = "-static" ]; then
	mkdir lib
	cp ./build/libelemd*.a ./lib/
else
	mkdir bin
	cp ./build/libelemd*.dylib ./bin/
fi

mkdir samples/hello_world
cp ./build/examples/hello_world/* ./samples/hello_world/

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path