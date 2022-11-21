#!/bin/zsh

cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE="Release" -DUSE_DOXYGEN="True";
cd build;
ninja;
cd ..;
echo "Building done!"
