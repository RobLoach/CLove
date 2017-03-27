/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

typedef struct {
  int major;
  int minor;
  int revision;
  const char* strVersion;
  char const * codename;
} love_Version;

love_Version const * love_getVersion(void);
