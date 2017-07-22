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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SAFE_FREE(o) {  if (o) { free(o); o = NULL; } }

#define CLOVE_ERROR(x) {  printf("CLove Error: %s \n ", x);  }
#define CLOVE_DEBUG(x) { printf("CLove Debug: %s %s %s %s %s \n", x , " at line " __LINE__, " in file  ", __FILE__ ); }

#ifndef USE_NATIVE
    #define USE_NATIVE 0x01
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#	define CLOVE_LINUX 1
#endif

#if defined(__APPLE__)
#	define CLOVE_MACOSX 1
#endif

#if defined(WIN32) || defined(_WIN32)
#	define CLOVE_WINDOWS 1
#endif

#if defined(EMSCRIPTEN) || defined(__EMSCRIPTEN__)
#  define CLOVE_WEB
#  define CLOVE_ES
#endif

#if defined(__ANDROID__) || defined(ANDROID)
# 	define CLOVE_ANDROID
#   define CLOVE_ES
#endif

#if defined(CLOVE_MACOSX) || defined(CLOVE_LINUX)
#   define CLOVE_UNIX
# 	define CLOVE_GL
#endif

#if defined(CLOVE_MACOSX) || defined(CLOVE_LINUX) || defined(CLOVE_WINDOWS)
#   define CLOVE_DESKTOP
# 	define CLOVE_GL
#endif

char* util_concatenate(const char* first, const char* second);

#endif
