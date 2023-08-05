#!/bin/sh
clang++ -o src/init.o -c src/init.cpp -g -O3 -std=c++14 -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Igodot-cpp/godot-headers
clang++ -o bin/libtest.so -shared src/init.o -Lgodot-cpp/bin -lgodot-cpp.windows.debug.64