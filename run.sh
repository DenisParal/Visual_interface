#!/bin/sh
cd build
rm -rf ./*
cmake ..
make
c++  -O0 -g CMakeFiles/main.dir/test/basic_drawing.cpp.o  -o main  -Wl,-rpath,/home/dparanic/Study/Visual_interface/build libMyProject.so /lib/x86_64-linux-gnu/libpthread.so.0 -lsfml-graphics -lsfml-window -lsfml-system
./main