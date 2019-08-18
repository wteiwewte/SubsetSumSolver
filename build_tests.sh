#!/bin/bash
BUILD_DIR=build
if [ ! -d "$BUILD_DIR" ]; then
	mkdir "$BUILD_DIR"
fi
cd "$BUILD_DIR"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../
cd ../
/usr/bin/cmake --build "$BUILD_DIR" --target Test -- -j 2
./build/tests/Test
