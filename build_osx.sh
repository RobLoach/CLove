#!/bin/sh

mkdir build &&
cd build &&
cmake ../ -BUILD_OSX=ON &&
make -j2 &&
make install DESTDIR="install"
