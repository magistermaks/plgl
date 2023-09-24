#!/bin/bash
set -x
g++ -std=c++20 -g -Wall -Wextra -Wno-unused-parameter -O0 lib/winx.c lib/glad.c src/main.cpp -I. -ldl -lGL -lX11 -lXcursor -o main && ./main
