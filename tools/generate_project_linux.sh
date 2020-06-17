#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

cd ..
rm -rf build
mkdir build
cd build

cmake ..

cd $oldpath