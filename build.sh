#!/bin/bash

if [ -d "build" ]; then
    rm -r build && mkdir build
else 
    mkdir build
fi

if [ $1 = "Debug" ]; then
    cd build && \
        cmake .. -DCMAKE_BUILD_TYPE=Debug && make
elif [ $1 = "Release" ]; then
    cd build && \
        cmake .. -DCMAKE_BUILD_TYPE=Release && make
fi