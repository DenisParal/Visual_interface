#!/bin/sh
cd build
rm -rf ./*
cmake ..
make

c++  -O0 -g CMakeFiles/functional_test.dir/test/functional_object_test.cpp.o  -o functional_test  -Wl,-rpath,/home/dparanic/Study/Visual_interface/build libMyProject.so -lsfml-graphics -lsfml-window -lsfml-system

./functional_test