#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

python3 ./install.py

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $old_path