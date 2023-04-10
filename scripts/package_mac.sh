#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
if [ "$1" = "-static" ]; then
	mkdir lib
	cp ./build/Release/libelemd*.a ./lib/

	# ------ Package Examples ------

	# Hello World
	mkdir -p samples/hello_world
	cp -R ./build/examples/hello_world/res ./samples/hello_world
	cp ./build/examples/hello_world/Release/hello-world ./samples/hello_world

	# Overview
	mkdir -p samples/overview
	cp -R ./build/examples/overview/res ./samples/overview
	cp ./build/examples/overview/Release/overview ./samples/overview

	# Game Of Life
	mkdir -p samples/game_of_life
	cp ./build/examples/game_of_life/Release/game-of-life ./samples/game_of_life

	# Platformer
	mkdir -p samples/platformer
	cp -R ./build/examples/platformer/res ./samples/platformer
	cp ./build/examples/platformer/Release/platformer ./samples/platformer

	# Benchmark
	mkdir -p samples/benchmark
	cp ./build/examples/benchmark/Release/benchmark ./samples/benchmark
	
	# Calculator
	mkdir -p samples/calculator
	cp -R ./build/examples/calculator/res ./samples/calculator
	cp ./build/examples/calculator/Release/calculator ./samples/calculator

	# Slack
	mkdir -p samples/slack
	cp -R ./build/examples/slack/res ./samples/slack
	cp ./build/examples/slack/Release/slack ./samples/slack

	# Roots of Darknes
	mkdir -p samples/roots_of_darknes
	cp -R ./examples/roots_of_darknes/res ./samples/roots_of_darknes
	cp ./build/examples/roots_of_darknes/Release/roots-of-darknes ./samples/roots_of_darknes
	
else
	mkdir bin
	cp ./build/Release/libelemd*.dylib ./bin/
fi

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path