#!/bin/bash
BUILD_DIR=cmake-build-debug
if [ ! -d "$BUILD_DIR" ]; then
	mkdir "$BUILD_DIR"
fi
cd "$BUILD_DIR"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../
cd ../
/usr/bin/cmake --build "$BUILD_DIR" --target Subset_Sum_Solver -- -j 2
