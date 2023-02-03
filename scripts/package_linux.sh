#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
if [ "$1" = "-static" ]; then
	mkdir lib
	cp ./build/Release/libelemd.a* ./lib/ 	

	# ------ Package Examples ------

	# Roots of Darknes
	mkdir -p samples/roots_of_darknes
	cp -R ./build/Release/examples/roots_of_darknes/res ./samples/roots_of_darknes
	cp ./build/Release/examples/roots_of_darknes/Release/roots-of-darknes ./samples/roots_of_darknes
	
else
	mkdir bin
	cp ./build/Release/libelemd.so* ./bin/
fi

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path
