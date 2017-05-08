/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <math.h>

#include "../image/imagedata.h"

#include "image.h"
#include "quad.h"

typedef struct {
	graphics_Quad* quads;
	image_ImageData* data;
	graphics_Image* image;
	const char* glyphs;
} graphics_BitmapFont;

void graphics_BitmapFont_new(graphics_BitmapFont* dst, char const* filename, char const* glyphs, float glyph_width, float glyph_height);

void graphics_BitmapFont_setGlyphOffsetX(float off, char const* glyph);
void graphics_BitmapFont_setGlyphOffsetY(float off, char const* glyph);
void graphics_BitmapFont_render(graphics_BitmapFont* dst, char const* text, int x, int y, float r, float sx, float sy, float ox, float oy, float kx, float ky);

void graphics_BitmapFont_free(graphics_BitmapFont* dst);

void graphics_BitmapFont_setFilter(graphics_BitmapFont *font, graphics_Filter const* filter);

void graphics_BitmapFont_getFilter(graphics_BitmapFont *font, graphics_Filter *filter);
