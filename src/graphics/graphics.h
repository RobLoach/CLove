/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <stdbool.h>

#include "image.h"
#include "quad.h"
#include "../math/vector.h"

void graphics_init(int width, int height, bool resizable, bool stats);

typedef enum {
    graphics_BlendMode_additive,
    graphics_BlendMode_alpha,
    graphics_BlendMode_subtractive,
    graphics_BlendMode_multiplicative,
    graphics_BlendMode_premultiplied,
    graphics_BlendMode_replace,
    graphics_BlendMode_screen
} graphics_BlendMode;


void graphics_setBackgroundColor(float red, float green, float blue, float alpha);
void graphics_setColor(float red, float green, float blue, float alpha);
float* graphics_getColor(void);
float* graphics_getBackgroundColor(void);
void graphics_clear(void);
void graphics_swap(void);
void graphics_drawArray(graphics_Quad const* quad, mat4x4 const* tr2d, GLuint ibo, GLuint count, GLenum type, GLenum indexType, float const * useColor, float ws, float hs);
void graphics_drawArray3d(graphics_Quad const* quad, mat4x4 const* tr3d, GLuint ibo, GLuint count, GLenum type, GLenum indexType, float const * useColor, float ws, float hs);


//Window
int graphics_getDisplayCount();
int graphics_setIcon(image_ImageData* imgd);
image_ImageData* graphics_getIcon();
int mouse_focus;
int graphics_hasMouseFocus();
int graphics_setMouseFocus(int value);
int focus;
int graphics_hasFocus();
int graphics_setFocus(int value);
int graphics_getWidth(void);
int graphics_getHeight(void);
int graphics_setTitle(const char* title);
int graphics_setMode(int width, int height,
        bool fullscreen, int min_size_x, int min_size_y, int max_size_x, int max_size_y, bool border,
        int x, int y);

const char* graphics_getDisplayName(int indx);
int* graphics_getDesktopDimension();
void graphics_setVersion(const char* version);
const char* graphics_getVersion();
int graphics_setMinSize(int x, int y);
int graphics_setBordless(bool value);
int graphics_setVsync(bool value);
int graphics_setWindow(bool value);
int graphics_setFullscreen(int value, const char* mode);
int graphics_isCreated();
void graphics_destroyWindow();
const char* graphics_getTitle();
int graphics_setPosition(int x, int y);
void graphics_set_look_at(float px, float py, float pz,float tx,float ty,float tz, float ux, float uy, float uz);
void graphics_set_camera_3d(float fov, float ratio, float zNear, float zFar);
void graphics_set_camera_2d(float left, float right, float bottom, float top, float zNear, float zFar);
void graphics_setColorMask(bool r, bool g, bool b, bool a);
void graphics_getColorMask(bool *r, bool *g, bool *b, bool *a);
graphics_BlendMode graphics_getBlendMode();
void graphics_setBlendMode(graphics_BlendMode mode);
void graphics_clearScissor(void);
void graphics_setScissor(int x, int y, int w, int h);
bool graphics_getScissor(int *x, int *y, int *w, int *h);
void graphics_reset(void);
