/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "love.h"

love_Version const * love_getVersion(void) {
  static const love_Version version = {
    .major = 0,
    .minor = 7,
    .revision = 0,
    .strVersion = "0.7.0",
    .codename = "Cake"
  };

  return &version;
}
