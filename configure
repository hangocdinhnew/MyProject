#!/bin/zsh

cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE="Release" -DUSE_DOXYGEN="True" -DUSE_CPPCHECK="True";
# shellcheck disable=SC2164
cd build;
ninja;
# shellcheck disable=SC2103
cd ..;
echo ""
echo "Building done!"
