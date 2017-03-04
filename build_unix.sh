#!/bin/sh

mkdir build &&
cd build && 
cmake ../ &&
make && 
make install DESTDIR="install"