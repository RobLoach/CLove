/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#define _POSIX_SOURCE
#include <string.h>
#include "font.h"
#include <stdlib.h>
#include <stdio.h>
#include "quad.h"
#include "graphics.h"
#include "../tools/utf8.h"
#include "../math/minmax.h"
#include "shader.h"
#include "batch.h"
#include "vera_ttf.c"
#include "../3rdparty/freetype/include/ft2build.h"
#include "../3rdparty/freetype/include/freetype.h"
#include "../3rdparty/freetype/include/ftglyph.h"
#include <stdio.h>
#include <string.h>

#include FT_GLYPH_H
static struct {
  mat4x4 tr2d;
  FT_GlyphSlot g;
  const char* name;
  GLuint vbo;
  GLuint ibo;
  GLuint vao;
  FT_Library ft;

  uint32_t cp; //message to take utf8 from
  int x; // message's X
  int storeX; // store X pos
  int storeY; // store Y pos
  int y; // message's Y
} moduleData;

static graphics_Vertex const imageVertices[] = {
  {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
};

static graphics_Quad const quad = {
  0.0f,0.0f,1.0f,1.0f
};

static unsigned char const imageIndices[] = { 0, 1, 2, 3 };
static const graphics_Filter defaultFilter = {
  .maxAnisotropy = 1.0f,
  .mipmapLodBias = 1.0f,
  .minMode = graphics_FilterMode_linear,
  .magMode = graphics_FilterMode_linear,
  .mipmapMode = graphics_FilterMode_none
};

static const graphics_Wrap defaultWrap = {
  .verMode = graphics_WrapMode_clamp,
  .horMode = graphics_WrapMode_clamp
};

int graphics_font_init(void) {
  int error = FT_Init_FreeType(&moduleData.ft);
  if(error){
      printf("%s \n","Could not init freetype");
      return 1;
    }

  glGenBuffers(1, &moduleData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STATIC_DRAW);

  glGenBuffers(1, &moduleData.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(imageIndices), imageIndices, GL_STATIC_DRAW);

  return 1;
}

void graphics_Font_free(graphics_Font* font) {
  FT_Done_Face(font->face);
  FT_Done_FreeType(moduleData.ft);
  glDeleteTextures(1,&font->tex);
  glDeleteBuffers(1, &moduleData.ibo);
  glDeleteBuffers(1, &moduleData.vbo);
}

static void graphics_Font_newTexture(graphics_Font* font) {
  /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &font->tex);
  glBindTexture(GL_TEXTURE_2D, font->tex);

  FT_Bitmap b = moduleData.g->bitmap;

  uint8_t *buf = malloc(2*b.rows*b.width);
  uint8_t *row = b.buffer;
  for(int i = 0; i < b.rows; ++i) {
      for(int c = 0; c < b.width; ++c) {
          buf[2*(i*b.width + c)    ] = 255;
          buf[2*(i*b.width + c) + 1] = row[c];
        }
      row += b.pitch;
    }
  
  graphics_Font_setWrap(font, &defaultWrap);
  graphics_Font_setFilter(font, &defaultFilter);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, moduleData.g->bitmap.width, moduleData.g->bitmap.rows, 0,
               GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buf);
  free(buf);
}


int graphics_Font_new(graphics_Font* font, char const* filename, int ptsize) {
  if(filename){
      FT_New_Face(moduleData.ft, filename, 0, &font->face);
      font->path = filename;
    }else
    	FT_New_Memory_Face(moduleData.ft, defaultFontData, defaultFontSize, 0, &font->face);

  font->size = ptsize;
  FT_Set_Pixel_Sizes(font->face, 0, font->size);

  moduleData.g = font->face->glyph;
  // Load first 128 characters of ASCII set
  for (GLubyte c = 0; c < 128; c++){
      // Load character glyph
      if (FT_Load_Char(font->face, c, FT_LOAD_RENDER))
        continue;

      graphics_Font_newTexture(font);
      // Now store character for later use
      character _character = {
        font->tex,
        moduleData.g->bitmap.width,
        moduleData.g->bitmap.rows,
        moduleData.g->bitmap_left,
        moduleData.g->bitmap_top,
        moduleData.g->advance.x,
        moduleData.g->advance.y
      };
      //moduleData.characters[c] = _character;
      font->characters[c] = _character;
    }
  return 0;
}

int graphics_Font_getWidth(graphics_Font* font, char const* line) {
  int sum;
  uint32_t cp;
  while(cp = utf8_scan(&line)) {
      if(font->w)
        sum = cp * font->w;
    }
  return sum;
}

void graphics_Font_printf(graphics_Font* font, char const* text, int px, int py, int limit, graphics_TextAlign align,
                          float r, float sx, float sy, float ox, float oy, float kx, float ky) {
  moduleData.x = 0;
  moduleData.storeX = 0;
  moduleData.storeY = 0;
  moduleData.y = font->face->ascender;

  graphics_Shader* shader = graphics_getShader();
  graphics_setDefaultShader();

  int count = 0;
  int wrapped = 0;
  glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_DYNAMIC_DRAW);
  while((moduleData.cp = utf8_scan(&text))) {
      font->ch = font->characters[moduleData.cp];
      glBindTexture(GL_TEXTURE_2D,font->ch.textureid);
	
		
      if (moduleData.storeX == 0)
        moduleData.storeX = px ;//- font->ch.sizex;
      if (moduleData.storeY == 0)
        moduleData.storeY = py + font->ch.sizey + 1;
				
      moduleData.x = px + font->ch.bearingx;
      moduleData.y = py - font->ch.bearingy;
		
		
      if ((wrapped == 0 && ++count >= limit)){
          px = moduleData.x - (((font->ch.advancex >> 6)) * (font->ch.sizex + font->ch.bearingx));
          px = moduleData.storeX;
          py = moduleData.storeY;
          count = 0;
          wrapped = 1;
        }

      moduleData.x = px + font->ch.bearingx;
      moduleData.y = py - font->ch.bearingy;
		
      if (moduleData.cp == '\n'){
          px = moduleData.x - (((font->ch.advancex >> 6)) * (font->ch.sizex + font->ch.bearingx));
          if (px < moduleData.storeX)
            px = moduleData.storeX;
          py += floor(font->ch.bearingy + 5.25f);
          continue;
        }

      m4x4_newTransform2d(&moduleData.tr2d, moduleData.x, moduleData.y, r, sx, sy, ox, oy, kx, ky);

      graphics_drawArray(&quad, &moduleData.tr2d,  moduleData.ibo, 4, GL_TRIANGLE_STRIP, GL_UNSIGNED_BYTE,
                         graphics_getColor(), quad.w * font->ch.sizex , quad.h * font->ch.sizey);

      font->w = (int )&font->glyph->bitmap.width;
      font->w = font->w >> 6;
      px += font->ch.advancex >> 6;
      py += font->ch.advancey >> 6;
    }
  graphics_setShader(shader);

}

void graphics_Font_print(graphics_Font* font, char const* text, int px, int py, float r, float sx, float sy, float ox, float oy, float kx, float ky) {
  moduleData.x = 0;
  moduleData.storeX = 0;
  moduleData.y = font->face->ascender;
  
  graphics_Shader* shader = graphics_getShader();
  graphics_setDefaultShader();

  glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_DYNAMIC_DRAW);
   while((moduleData.cp = utf8_scan(&text))) {
      font->ch = font->characters[moduleData.cp];
      glBindTexture(GL_TEXTURE_2D,font->ch.textureid);

      if (moduleData.storeX == 0)
        moduleData.storeX = px; //- font->ch.sizex;

      moduleData.x = px + font->ch.bearingx;
      moduleData.y = py - font->ch.bearingy;

      if (moduleData.cp == '\n'){
          px = moduleData.x - (((font->ch.advancex >> 5)) * (font->ch.sizex + font->ch.bearingx));
          if (px < moduleData.storeX)
            px = moduleData.storeX;
          py += floor(font->ch.bearingy + 5.25f);
          continue;
        }

      m4x4_newTransform2d(&moduleData.tr2d, moduleData.x, moduleData.y, r, sx, sy, ox, oy, kx, ky);

      graphics_drawArray(&quad, &moduleData.tr2d,  moduleData.ibo, 4, GL_TRIANGLE_STRIP, GL_UNSIGNED_BYTE,
                         graphics_getColor(), quad.w * font->ch.sizex , quad.h * font->ch.sizey);

      font->w = (int )&font->glyph->bitmap.width;
      font->w = font->w >> 6;
      px += font->ch.advancex >> 6;
      py += font->ch.advancey >> 6;
    }
  graphics_setShader(shader);
}

void graphics_Font_setFilter(graphics_Font* font, graphics_Filter const* filter) {
  graphics_Texture_setFilter(font->tex, filter);
}

void graphics_Font_getFilter(graphics_Font *font, graphics_Filter *filter) {
  graphics_Texture_getFilter(font->tex, filter);
}

void graphics_Font_setWrap(graphics_Font *font, graphics_Wrap const* wrap) {
  glBindTexture(GL_TEXTURE_2D, font->tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap->horMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap->verMode);
}

int graphics_Font_getDescent(graphics_Font* font) {
  return font->face->descender;
}

int graphics_Font_getBaseline(graphics_Font* font) {
  return floor(font->glyph->bitmap.rows / 1.25f + .5f);
}

int graphics_Font_getAscent(graphics_Font* font) {
  return font->face->ascender;
}

int graphics_Font_getHeight(graphics_Font* font){
  return moduleData.g->bitmap.rows;
}

