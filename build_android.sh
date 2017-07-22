#!/bin/sh

#Creates the lib used by android to make the .apk
mkdir -p build_android &&
cd build_android &&
cmake     -BUILD_ANDROID=TRUE  \
		  -DCMAKE_TOOLCHAIN_FILE="android.toolchain.cmake" \
		  -DANDROID_NDK=${ANDROID_NDK} ../ &&
make
