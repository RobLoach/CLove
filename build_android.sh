#!/bin/sh

#Creates the lib used by android to make the .apk
mkdir build_android
cd build_android
cmake     -BUILD_ANDROID=ON  \
		  -DCMAKE_TOOLCHAIN_FILE="android.toolchain.cmake" \
		  -DANDROID_NDK=${ANDROID_NDK} ../ &&
make
