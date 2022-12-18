#!/bin/zsh

cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE="Release" -DUSE_DOXYGEN="True" -DUSE_CPPCHECK="True";
cd build;
ninja;
cd ..;
echo ""
echo "Building done!"
