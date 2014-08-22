#!/bin/bash

BUILD_DIR="b"

if [[ -e "env.sh" ]]; then
	. env.sh
fi

if [[ ! -e "$BUILD_DIR" ]]; then
	mkdir "$BUILD_DIR";
	cd "$BUILD_DIR";
	cmake ..
	cd ..
fi;

cd "$BUILD_DIR";
make && ./lib/test_enums.cpp
#ctest --verbose
