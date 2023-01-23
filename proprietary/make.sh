#!/bin/sh
./patch_lib.sh
gcc -c main.c -o main.o
gcc main.o lib/libftd3xx-patched-static.a -o a.out -ludev -lstdc++
# gcc main.o lib/libftd3xx-static.a -o a.out -ludev -lstdc++
