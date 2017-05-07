/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "bitmap_font.h"

#include <stdlib.h>
#include <string.h>

#include "../tools/utf8.h"
#include "shader.h"

typedef struct
{
	uint32_t word;
	uint16_t index;
} glyphs_indexer;

static struct {
	glyphs_indexer* indexer_map;
	uint16_t indexer_map_size;
} moduleData;

void graphics_BitmapFont_new(graphics_BitmapFont* dst, char const* filename,char const* glyphs, float glyph_width, float glyph_height)
{
	moduleData.indexer_map = malloc(sizeof(glyphs_indexer) * strlen(glyphs)+1);

	dst->image = malloc(sizeof(graphics_Image));
	dst->data = malloc(sizeof(image_ImageData));

	//TODO add error checking
	int err = image_ImageData_new_with_filename(dst->data,filename);

	graphics_Image_new_with_ImageData(dst->image, dst->data);

	int img_width = image_ImageData_getWidth(dst->data);
	int img_height = image_ImageData_getHeight(dst->data);
	int no_glyhs = moduleData.indexer_map_size = strlen(glyphs);

	dst->quads = malloc(sizeof(graphics_Quad) * no_glyhs + 1);

	// Get all the glyphs from the font
	uint16_t i = 0;
	uint32_t get_w;
	while(get_w = utf8_scan(&glyphs))
	{
		graphics_Quad_newWithRef(&dst->quads[i], glyph_width * i, 0, glyph_width, img_height, img_width, img_height);
		moduleData.indexer_map[i].word = get_w;
		moduleData.indexer_map[i].index = i;
		i++;
	}

}

static glyphs_indexer find_glyph(uint32_t what)
{
	for (uint16_t i = 0; i < moduleData.indexer_map_size; i++)
	{
		uint32_t get_word = moduleData.indexer_map[i].word;
		uint16_t get_index = moduleData.indexer_map[i].index;
		if (get_word == what)
			return moduleData.indexer_map[i];
	}
	glyphs_indexer map;
	return map;
}

void graphics_BitmapFont_render(graphics_BitmapFont* dst, char const* text, int x, int y, float r, float sx, float sy, float ox, float oy, float kx, float ky)
{

	uint32_t g;
	int i = 0;
	while(g = utf8_scan(&text))
	{
		glyphs_indexer map = find_glyph(g);
		if (map.word == g) {
			//printf("%c %d \n",map.word, map.index);
			graphics_Image_draw(dst->image, &dst->quads[map.index], x + 12 * i, y, r, sx, sy, ox, oy, kx, ky);
			i++;
		}
	}
}

void graphics_BitmapFont_free(graphics_BitmapFont* dst)
{
	free(dst->data);
	free(dst->image);
	free(dst->quads);
	free(moduleData.indexer_map);
}

void graphics_BitmapFont_setFilter(graphics_BitmapFont *font, graphics_Filter const* filter)
{

}

void graphics_BitmapFont_getFilter(graphics_BitmapFont *font, graphics_Filter *filter)
{

}


