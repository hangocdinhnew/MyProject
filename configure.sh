#!/bin/zsh

cmake -B build -G "Ninja";
cd build;
ninja;
cd ..
echo "Building done!";