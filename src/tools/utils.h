/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

#define SAFE_FREE(o) {  if (o) { free(o); o = NULL; } }

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
#if defined(CLOVE_MACOSX) || defined(CLOVE_LINUX) || defined(CLOVE_WINDOWS)
#   define CLOVE_DESKTOP
# 	define CLOVE_GL
#endif
#if defined(EMSCRIPTEN)
#  define CLOVE_WEB
#  define CLOVE_ES
#endif
#if defined(__ANDROID__) || ANDROID
# 	define CLOVE_ANDROID
#   define CLOVE_ES
#endif
#endif
