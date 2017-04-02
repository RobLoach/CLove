#!/bin/sh

mkdir build &&
cd build &&
cmake -BUILD_LINUX=ON ../ &&
make -j2 &&
make install DESTDIR="install"
