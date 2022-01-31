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

	# Overview
	mkdir -p samples/overview
	cp -R ./build/examples/overview/res ./samples/overview
	cp ./build/examples/overview/overview ./samples/overview

	# Game Of Life
	mkdir -p samples/game_of_life
	cp ./build/examples/game_of_life/game-of-life ./samples/game_of_life

	# Platformer
	mkdir -p samples/platformer
	cp -R ./build/examples/platformer/res ./samples/platformer
	cp ./build/examples/platformer/platformer ./samples/platformer

	# Benchmark
	mkdir -p samples/benchmark
	cp ./build/examples/benchmark/benchmark ./samples/benchmark
else
	mkdir bin
	cp ./build/libelemd.so* ./bin/
fi

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path