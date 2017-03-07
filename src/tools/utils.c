/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdio.h>

#ifndef USE_NATIVE
    #define USE_NATIVE 0x01
#endif

#if defined(WIN32) || defined(_WIN32)
#	define CLOVE_WINDOWS 1
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
#	define CLOVE_LINUX 1
#endif
#if defined(__APPLE__)
#	define CLOVE_MACOSX 1
#endif
#if defined(CLOVE_MACOSX) || defined(CLOVE_LINUX)
#   define CLOVE_UNIX
#endif
