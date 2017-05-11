#!/bin/sh

mkdir build &&
cd build &&
cmake ../ &&
make -j2 &&
make install DESTDIR="install"
