/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../image/imagedata.h"
#include "quad.h"
#ifdef EMSCRIPTEN
#include <GLES2/gl2.h>
#else
#include "../3rdparty/glew/include/GL/glew.h"
#endif

#include "gltools.h"

typedef enum {
  graphics_WrapMode_clamp = GL_CLAMP_TO_EDGE,
  graphics_WrapMode_repeat = GL_REPEAT
} graphics_WrapMode;


typedef struct {
  graphics_WrapMode verMode;
  graphics_WrapMode horMode;
} graphics_Wrap;


typedef struct {
  GLuint texID;
  int width;
  int height;
  GLuint imageVBO;
  GLuint imageIBO;
} graphics_Image;


void graphics_image_init(void);
void graphics_Image_new_with_ImageData(graphics_Image *dst, image_ImageData *data);
void graphics_Image_new(graphics_Image *dst);
void graphics_Image_free(graphics_Image *obj);
void graphics_Image_setFilter(graphics_Image *img, graphics_Filter const* filter);
void graphics_Image_getFilter(graphics_Image *img, graphics_Filter *filter);
void graphics_Image_setWrap(graphics_Image *img, graphics_Wrap const* wrap);
void graphics_Image_getWrap(graphics_Image *img, graphics_Wrap *wrap);
void graphics_Image_refresh(graphics_Image *img, image_ImageData const* data);
void graphics_Image_draw(graphics_Image const* image, graphics_Quad const* quad, float x, float y, float z,
                         float r, float rx, float ry, float rz,
                         float sx, float sy, float sz,
                         float ox, float oy,
                         float kx, float ky);
