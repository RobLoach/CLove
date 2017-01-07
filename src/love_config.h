/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdbool.h>

#pragma once

typedef struct {
  int width;
  int height;
  const char* title;
  const char* version;
  bool vsync;
  bool resizable;
  bool window;
  bool bordless;
  int minwidth, minheight;
  int x, y;
  int stats;
} love_WindowConfig;

typedef struct {
  love_WindowConfig window;
} love_Config;
