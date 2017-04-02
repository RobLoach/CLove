#!/bin/sh

mkdir build &&
cd build &&
cmake -BUILD_OSX=TRUE ../ &&
make -j2 &&
make install DESTDIR="install"
