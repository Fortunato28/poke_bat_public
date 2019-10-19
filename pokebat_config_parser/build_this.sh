#!/bin/sh

BUILD_DIR="_result"

if [ ! -d $BUILD_DIR  ]; then
    mkdir $BUILD_DIR
fi

cd _result
cmake ../
make
