#!/bin/bash
if [ $1 = "Debug" ]; then
    if [ $(test -d build) ]; then
        rm -r build
    else 
        mkdir build
    fi
    
    cd build && \
        cmake .. -DCMAKE_BUILD_TYPE=Debug && make
elif [ $1 = "Release" ]; then
    if [ $(test -d build) ]; then
        rm -r build
    else 
        mkdir build
    fi
    
    cd build && \
        cmake .. -DCMAKE_BUILD_TYPE=Release && make
fi