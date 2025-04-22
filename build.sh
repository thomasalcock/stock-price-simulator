#!/bin/bash
if [ $1 = "Debug" ]; then
    rm -r build && mkdir -p build/debug && \
        cd build/debug && \
        cmake ../../ -DCMAKE_BUILD_TYPE=Debug && make
elif [ $1 = "Release" ]; then
    rm -r build && mkdir build/release && \
        cd build/release && \
        cmake ../../ -DCMAKE_BUILD_TYPE=Release && make
fi