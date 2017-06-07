/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "bitmap_font.h"
#include "shader.h"
#include "batch.h"

#include <stdlib.h>
#include <string.h>

#include "../image/imagedata.h"
#include "../tools/utf8.h"

typedef struct
{
    uint16_t width;
    uint16_t height;
    float  offsetx;
    float  offsety;
    uint32_t word;
    uint16_t index;
} glyphs_indexer;

static struct
{
    glyphs_indexer* indexer_map;
    uint16_t indexer_map_size;
    uint16_t glyph_width;
    uint16_t glyph_height;

    graphics_Batch* batches;
    uint32_t batchcount;
    uint32_t batchsize;
} moduleData;

void graphics_BitmapFont_new(graphics_BitmapFont* dst, char const* filename, char const* glyphs, float glyph_width, float glyph_height)
{
    moduleData.indexer_map = malloc(sizeof(glyphs_indexer) * strlen(glyphs)+1);

    dst->image = malloc(sizeof(graphics_Image));
    dst->data = malloc(sizeof(image_ImageData));

    int err = image_ImageData_new_with_filename(dst->data, filename);
    if (err == 0)
        printf("%s \n", dst->data->error_msg);

    graphics_Image_new_with_ImageData(dst->image, dst->data);

    int img_width = image_ImageData_getWidth(dst->data);
    int img_height = image_ImageData_getHeight(dst->data);
    int no_glyphs = moduleData.indexer_map_size = strlen(glyphs);

    dst->quads = malloc(sizeof(graphics_Quad) * no_glyphs + 1);

    // In case we did not specify these two we calculate them manually
    glyph_width = glyph_width == 0 ? (img_width / no_glyphs) : glyph_width;
    glyph_height = glyph_height == 0 ? img_height : glyph_height;

    // Get all the glyphs from the font
    uint16_t i = 0;
    uint32_t get_w;

    moduleData.batches = malloc(sizeof(graphics_Batch) * no_glyphs);
    moduleData.batchcount = no_glyphs;

    while(get_w = utf8_scan(&glyphs))
    {
        graphics_Quad_newWithRef(&dst->quads[i], glyph_width * i, 0, glyph_width, glyph_height, img_width, img_height);
        moduleData.indexer_map[i].word = get_w;
        moduleData.indexer_map[i].index = i;
        moduleData.indexer_map[i].offsetx = 0;
        moduleData.indexer_map[i].offsety = 0;
        moduleData.glyph_width = glyph_width;
        moduleData.glyph_height = glyph_height;

        graphics_Batch_new(&moduleData.batches[i], dst->image, no_glyphs, graphics_BatchUsage_static);
        graphics_Batch_bind(&moduleData.batches[i]);
        i++;
    }
}

void graphics_BitmapFont_setGlyphOffsetX(float off, char const* glyph)
{
    uint32_t convert_glyph = utf8_scan(&glyph);
    for (uint16_t i = 0; i < moduleData.indexer_map_size; i++)
    {
        if (moduleData.indexer_map[i].word == convert_glyph)
        {
            moduleData.indexer_map[i].offsetx = off;
        }
    }
}

void graphics_BitmapFont_setGlyphOffsetY(float off, char const* glyph)
{
    uint32_t convert_glyph = utf8_scan(&glyph);
    for (uint16_t i = 0; i < moduleData.indexer_map_size; i++)
    {
        if (moduleData.indexer_map[i].word == convert_glyph)
            moduleData.indexer_map[i].offsety = off;
    }
}

static glyphs_indexer find_glyph(uint32_t what)
{
    for (uint16_t i = 0; i < moduleData.indexer_map_size; i++)
    {
        uint32_t get_word = moduleData.indexer_map[i].word;
        //uint16_t get_index = moduleData.indexer_map[i].index;
        if (get_word == what)
            return moduleData.indexer_map[i];
    }
    glyphs_indexer map;
    return map;
}

void graphics_BitmapFont_render(graphics_BitmapFont* dst, char const* text, int x, int y, float r, float sx, float sy, float ox, float oy, float kx, float ky)
{
    graphics_Shader* shader = graphics_getShader();
    graphics_setDefaultShader();

    uint32_t g;
    int i = 0;
    while(g = utf8_scan(&text))
    {
        glyphs_indexer map = find_glyph(g);
        if (map.word == g) {
            //printf("%c %d %d %d \n", map.word, map.index, map.offsetx, map.offsety );
            graphics_Batch_add(&moduleData.batches[i], &dst->quads[map.index], map.offsetx + moduleData.glyph_width * i, map.offsety, 0, 1, 1, 0, 0, 0, 0);
            i++;
        }
    }

    for (int i = 0; i < moduleData.batchcount; i++)
    {
        graphics_Batch_unbind(&moduleData.batches[i]);
        graphics_Batch_draw(&moduleData.batches[i], x, y, r, sx, sy, ox, oy, kx, ky);
    }

    graphics_setShader(shader);
}

void graphics_BitmapFont_free(graphics_BitmapFont* dst)
{
    graphics_Image_free(dst->image);
    graphics_Batch_free(moduleData.batches);
    image_ImageData_free(dst->data);
    free(dst->quads);
    free(moduleData.indexer_map);
}


