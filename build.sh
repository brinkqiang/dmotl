#!/bin/bash


echo sudo apt install unixodbc-dev

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=relwithdebinfo ..
cmake --build . --config relwithdebinfo

cd ..

# popd

# echo continue && read -n 1
