#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
if [ "$1" = "-static" ]; then
	mkdir lib
	cp ./build/libelemd.a* ./lib/ 	

	# ------ Package Examples ------

	# Hello World
	mkdir -p samples/hello_world
	cp -R ./build/examples/hello_world/res ./samples/hello_world
	cp ./build/examples/hello_world/hello-world ./samples/hello_world
	#cp ./build/examples/hello_world/libelemd.so ./samples/hello_world

	# Game Of Life
	mkdir -p samples/game_of_life
	cp -R ./build/examples/game_of_life/res ./samples/game_of_life
	cp ./build/examples/game_of_life/game-of-life ./samples/game_of_life
	#cp ./build/examples/game_of_life/libelemd.so ./samples/game_of_life
else
	mkdir bin
	cp ./build/libelemd.so* ./bin/
fi

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path