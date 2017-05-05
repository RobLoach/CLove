/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#   Big thanks to Florian Kesseler
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

// http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_GlyphSlotRec

#define _POSIX_SOURCE
#include <string.h>
#include <tgmath.h>
#include "font.h"
#include <stdlib.h>
#include "quad.h"
#include "graphics.h"
#include "../tools/utf8.h"
#include "../math/minmax.h"
#include "shader.h"
#include "batch.h"
#include "vera_ttf.c"

#include FT_GLYPH_H

static const int GlyphTexturePadding = 1;

static struct {
    FT_Library ft;
    int wordcount;
    graphics_Batch *batches;
    int batchcount;
    int batchsize;
} moduleData;


void graphics_font_init() {
    int error = FT_Init_FreeType(&moduleData.ft);
    if (error)
        printf("Error, could not initialize freetype! \n");
    moduleData.batchcount = 0;
    moduleData.batches = NULL;
    moduleData.batchsize = 0;
}

void graphics_GlyphMap_newTexture(graphics_GlyphMap *map) {
    map->textures = realloc(map->textures, sizeof(GLuint) * (map->numTextures + 1));
    glGenTextures(1, &map->textures[map->numTextures]);
    glBindTexture(GL_TEXTURE_2D, map->textures[map->numTextures]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, map->textureWidth, map->textureHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(map->numTextures > 0) {
        graphics_Filter filter;
        graphics_Texture_getFilter(map->textures[map->numTextures-1], &filter);
        graphics_Texture_setFilter(map->textures[map->numTextures], &filter);
    }
    ++map->numTextures;
}

graphics_Glyph const* graphics_Font_findGlyph(graphics_Font *font, unsigned unicode) {
    // only 256 
    unsigned idx = unicode & 0xFF;
    graphics_GlyphSet *set = &font->glyphs.glyphs[idx];

    // Do we already have the glyph?
    int i = 0;
    for(; i < set->numGlyphs; ++i) {
        graphics_Glyph const* glyph = &set->glyphs[i];
        // The list is sorted, break early if possible
        if(glyph->code > unicode) {
            break;
        } else if(glyph->code == unicode) {
            return glyph;
        }
    }

    // Not found -> insert before last checked position
    if(i > 0){
        --i;
    }

    // Allocate new glyphs, copy old glyphs over and make space for the new one in the
    // right position (leave space at insert point)
    graphics_Glyph * newGlyphs = malloc(sizeof(graphics_Glyph) * (set->numGlyphs + 1));
    memcpy(newGlyphs, set->glyphs, i * sizeof(graphics_Glyph));
    memcpy(newGlyphs + i + 1, set->glyphs + i, (set->numGlyphs - i) * sizeof(graphics_Glyph));
    free((void*)set->glyphs);
    set->glyphs = newGlyphs;
    ++set->numGlyphs;

    // The storage for the new glyph data has been prepared in the last step,
    // we can just use it.
    graphics_Glyph * newGlyph = newGlyphs + i;

    // Load and render the glyph at the desired size
    unsigned index = FT_Get_Char_Index(font->face, unicode);
    FT_Load_Glyph(font->face, index, FT_LOAD_DEFAULT);
    FT_Glyph g;
    FT_Get_Glyph(font->face->glyph, &g);
    FT_Glyph_To_Bitmap(&g, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph fg = (FT_BitmapGlyph)g;
    FT_Bitmap b = fg->bitmap;

    uint8_t *buf = malloc(2*b.rows*b.width);
    uint8_t *row = b.buffer;
    for(int i = 0; i < b.rows; ++i) {
        for(int c = 0; c < b.width; ++c) {
            buf[2*(i*b.width + c)    ] = 255;
            buf[2*(i*b.width + c) + 1] = row[c];
        }
        row += b.pitch;
    }

    // Is the current row in the texture too full for the glyph?
    if(font->glyphs.currentX + GlyphTexturePadding + b.width > font->glyphs.textureWidth) {
        font->glyphs.currentX = GlyphTexturePadding;
        font->glyphs.currentY += font->glyphs.currentRowHeight;
        font->glyphs.currentRowHeight = GlyphTexturePadding;
    }

    if(font->glyphs.currentY + GlyphTexturePadding + b.rows > font->glyphs.textureHeight) {
        font->glyphs.currentX = GlyphTexturePadding;
        font->glyphs.currentY = GlyphTexturePadding;
        font->glyphs.currentRowHeight = GlyphTexturePadding;
        graphics_GlyphMap_newTexture(&font->glyphs);
    }

    // TODO: create new texture if current texture is full

    // Bind current texture and upload data to the appropriate position.
    // This assumes pixel unpack alignment is set to 1 (glPixelStorei)
    glBindTexture(GL_TEXTURE_2D, font->glyphs.textures[font->glyphs.numTextures-1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, font->glyphs.currentX, font->glyphs.currentY,
            b.width, b.rows, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buf);

    // Store geometric information for the glyph
    newGlyph->code = unicode;
    newGlyph->bearingX        = font->face->glyph->metrics.horiBearingX >> 6;
    newGlyph->bearingY        = font->face->glyph->metrics.horiBearingY >> 6;
    newGlyph->advance         = font->face->glyph->metrics.horiAdvance >> 6;
    newGlyph->textureIdx      = font->glyphs.numTextures - 1;
    newGlyph->textureCoords.x = (float)font->glyphs.currentX / (float)font->glyphs.textureWidth;
    newGlyph->textureCoords.y = (float)font->glyphs.currentY / (float)font->glyphs.textureHeight;
    newGlyph->textureCoords.w = (float)b.width / (float)font->glyphs.textureWidth;
    newGlyph->textureCoords.h = (float)b.rows  / (float)font->glyphs.textureHeight;

    // Advance to render position for next glyph
    font->glyphs.currentX += b.width + GlyphTexturePadding;
    font->glyphs.currentRowHeight = max(font->glyphs.currentRowHeight, b.rows + GlyphTexturePadding);

    free(buf);
    FT_Done_Glyph(g);

    return newGlyph;
}

static int const TextureWidth =  128;
static int const TextureHeight = 128;

int graphics_Font_new(graphics_Font *dst, char const* filename, int ptsize) {
    int error;
    if(filename) {
        error = FT_New_Face(moduleData.ft, filename, 0, &dst->face);
    } else {
        error = FT_New_Memory_Face(moduleData.ft, defaultFontData, defaultFontSize, 0, &dst->face);
    }
    if (error != 0) {
        if ( error == FT_Err_Unknown_File_Format ) 
            printf("%s \n","Error, the font file could be opened and read");
        else 
            printf("%s \n", "Error, the font file could not be opened or read");
    }

    FT_Set_Pixel_Sizes(dst->face, 0, ptsize);

    memset(&dst->glyphs, 0, sizeof(graphics_GlyphMap));
    
    dst->glyphs.textureWidth = TextureWidth;
    dst->glyphs.textureHeight = TextureHeight;
    
    // create a new texture with no data in it.
    graphics_GlyphMap_newTexture(&dst->glyphs);

    dst->height = dst->face->size->metrics.height >> 6;
    //The ascender is the vertical distance from the horizontal baseline to the highest ‘character’ coordinate in a font face. 
    dst->ascent = dst->face->size->metrics.ascender >> 6;
    //The descender is the vertical distance from the horizontal baseline to the lowest ‘character’ coordinate in a font face.
    dst->descent = dst->face->size->metrics.descender >> 6;
    dst->lineHeight = 1.0f;
    dst->batchsize = 1;

    return 0;
}


int graphics_Font_getWrap(graphics_Font * font, char const* text, int width, char ** wrapped) {
    // TODO
    return 0;
}

static void prepareBatches(graphics_Font* font, int chars) {
    int newSize = max(chars, moduleData.batchsize);

    /*
     * This will be called only once to set up the only texture used for batch which will contain
     * all the text from print and printf functions.
     */
    if(font->glyphs.numTextures > moduleData.batchcount) {
        moduleData.batches = realloc(moduleData.batches, font->glyphs.numTextures * sizeof(graphics_Batch));
        for(int i = moduleData.batchcount; i < font->glyphs.numTextures; ++i) {
            graphics_Image *img = malloc(sizeof(graphics_Image));
            img->texID = font->glyphs.textures[i];
            img->width = font->glyphs.textureWidth;
            img->height = font->glyphs.textureHeight;
            graphics_Batch_new(&moduleData.batches[i], img, newSize, graphics_BatchUsage_stream);
            graphics_Batch_bind(&moduleData.batches[i]);
        }
    }

    /*
     * If batchsize is smaller than the size of the text we want to print then we
     * need to increas the size of the batch and update the rest of components
     */
    if(font->batchsize < newSize) {
        for(int i = 0; i < moduleData.batchcount; ++i) {
            graphics_Batch_bind(&moduleData.batches[i]);
            graphics_Batch_changeBufferSize(&moduleData.batches[i], newSize);
            ((graphics_Image*)moduleData.batches[i].texture)->texID = font->glyphs.textures[i];
            ((graphics_Image*)moduleData.batches[i].texture)->width = font->glyphs.textureWidth;
            ((graphics_Image*)moduleData.batches[i].texture)->height = font->glyphs.textureHeight;
        }
    } else {
        /*
         * In case we do not need to increase the size of the batch we just update the
         * components
         */
        for(int i = 0; i < moduleData.batchcount; ++i) {
            graphics_Batch_bind(&moduleData.batches[i]);
            graphics_Batch_clear(&moduleData.batches[i]);
            ((graphics_Image*)moduleData.batches[i].texture)->texID = font->glyphs.textures[i];
            ((graphics_Image*)moduleData.batches[i].texture)->width = font->glyphs.textureWidth;
            ((graphics_Image*)moduleData.batches[i].texture)->height = font->glyphs.textureHeight;
        }
    }

    moduleData.batchcount = font->glyphs.numTextures;
    moduleData.batchsize = newSize;
}

void graphics_Font_render(graphics_Font* font, char const* text, int px, int py, float r, float sx, float sy, float ox, float oy, float kx, float ky) {
    prepareBatches(font, strlen(text));
    uint32_t cp;
    graphics_Shader* shader = graphics_getShader();
    graphics_setDefaultShader();
    int x = 0;
    int y = font->ascent;
    while((cp = utf8_scan(&text))) {
         // This will create the glyph if required
        graphics_Glyph const* glyph = graphics_Font_findGlyph(font, cp);

        if(cp == '\n') {
            x = 0;
            y += floor(font->height * font->lineHeight + 0.5f);
            continue;
        }
        
        graphics_Batch_add(&moduleData.batches[glyph->textureIdx], &glyph->textureCoords, x+glyph->bearingX, y-glyph->bearingY, 0, 1, 1, 0, 0, 0, 0);

        x += glyph->advance;
    }

    for(int i = 0; i < moduleData.batchcount; ++i) {
        graphics_Batch_unbind(&moduleData.batches[i]);
        graphics_Batch_draw(&moduleData.batches[i], px, py, r, sx, sy, ox, oy, kx, ky);
    }

    graphics_setShader(shader);
}


void graphics_Font_printf(graphics_Font* font, char const* text, int px, int py, int limit, graphics_TextAlign align, float r, float sx, float sy, float ox, float oy, float kx, float ky) {

}

int graphics_Font_getAscent(graphics_Font const* font) {
    return font->ascent;
}

int graphics_Font_getDescent(graphics_Font const* font) {
    return font->descent;
}

int graphics_Font_getBaseline(graphics_Font const* font) {
    return floor(font->height / 1.25f + 0.5f);
}

int graphics_Font_getHeight(graphics_Font const* font) {
    return font->height;
}

int graphics_Font_getWidth(graphics_Font * font, char const* line) {
    uint32_t uni;
    int width=0;
    while((uni = utf8_scan(&line))) {
        graphics_Glyph const* g = graphics_Font_findGlyph(font, uni);
        width += g->advance;
    }
    return width;
}

void graphics_Font_setFilter(graphics_Font *font, graphics_Filter const* filter) {
    for(int i = 0; i < font->glyphs.numTextures; i++) {
        graphics_Texture_setFilter(font->glyphs.textures[i], filter);
    }
}

void graphics_Font_getFilter(graphics_Font *font, graphics_Filter *filter) {
    graphics_Texture_getFilter(font->glyphs.textures[0], filter);
}

void graphics_GlyphMap_free(graphics_GlyphMap* map) {
    glDeleteTextures(map->numTextures, map->textures);
    free(map->textures);
}

void graphics_Font_free(graphics_Font *obj) {
    FT_Done_Face(obj->face);
    graphics_GlyphMap_free(&obj->glyphs);
}


