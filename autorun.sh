#!/bin/sh

set -e

# Clean
if [ "$1" = "clean" ]; then
    rm -rf build
    exit 0
fi

# Configure CMake
cmake -B ./build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build ./build

# Run
if [ $? -eq 0 ]; then
    if [ "$1" = "run" ]; then
        ./build/xale-db-debug
    elif [ "$1" = "tests" ]; then
        ./build/xale-db-tests
    fi
else
    exit 1
fi
