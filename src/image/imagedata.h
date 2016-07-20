/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include <stdint.h>

typedef struct {
  int w;
  int h;
  int x;
  int y;
  int c;
  const char* path;
  unsigned char *surface;
} image_ImageData;

char const* image_error(void);

void image_ImageData_new_with_size(image_ImageData *dst, int width, int height);

int image_ImageData_new_with_filename(image_ImageData *dst, char const* filename);

int image_ImageData_getWidth(image_ImageData *dst);

int image_ImageData_getHeight(image_ImageData *dst);

int image_ImageData_getChannels(image_ImageData *dst);

int image_ImageData_getPixel(image_ImageData *dst, int x, int y);

int image_ImageData_setPixel(image_ImageData *dst, int x, int y, unsigned char r, unsigned char g, unsigned char	b, unsigned char a); 

unsigned char* image_ImageData_getSurface(image_ImageData *dst);

void image_ImageData_free(image_ImageData *data);

void image_init();
