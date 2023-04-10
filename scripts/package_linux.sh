#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
if [ "$1" = "-static" ]; then
	mkdir lib
	cp ./build/Release/libelemd.a* ./lib/ 	

	# ------ Package Examples ------

	# Hello World
	mkdir -p samples/hello_world
	cp -R ./build/Release/examples/hello_world/res ./samples/hello_world
	cp ./build/Release/examples/hello_world/hello-world ./samples/hello_world

	# Overview
	mkdir -p samples/overview
	cp -R ./build/Release/examples/overview/res ./samples/overview
	cp ./build/Release/examples/overview/overview ./samples/overview

	# Game Of Life
	mkdir -p samples/game_of_life
	cp ./build/Release/examples/game_of_life/game-of-life ./samples/game_of_life

	# Platformer
	mkdir -p samples/platformer
	cp -R ./build/Release/examples/platformer/res ./samples/platformer
	cp ./build/Release/examples/platformer/platformer ./samples/platformer

	# Benchmark
	mkdir -p samples/benchmark
	cp ./build/Release/examples/benchmark/benchmark ./samples/benchmark
	
	# Calculator
	mkdir -p samples/calculator
	cp -R ./build/Release/examples/calculator/res ./samples/calculator
	cp ./build/Release/examples/calculator/calculator ./samples/calculator

	# Slack
	mkdir -p samples/slack
	cp -R ./build/Release/examples/slack/res ./samples/slack
	cp ./build/Release/examples/slack/slack ./samples/slack

	# Roots of Darknes
	mkdir -p samples/roots_of_darknes
	cp -R ./examples/roots_of_darknes/res ./samples/roots_of_darknes
	cp ./build/Release/examples/roots_of_darknes/roots-of-darknes ./samples/roots_of_darknes
	
else
	mkdir bin
	cp ./build/Release/libelemd.so* ./bin/
fi

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path
