#!/bin/zsh

cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE="Release";
cd build;
ninja;
cd ..;
echo "Building done!"
