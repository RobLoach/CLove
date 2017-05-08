/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../3rdparty/lua/lauxlib.h"
#include "graphics_bitmapfont.h"

#include "tools.h"
#include "graphics.h"

#include "../graphics/bitmap_font.h"
#include "../graphics/matrixstack.h"
#include "../graphics/shader.h"
#include "../graphics/graphics.h"

static struct {
	int mt;
	int ref;
    int currentFontRef;
	graphics_BitmapFont* currentFont;
} moduleData;

int l_graphics_newBitmapFont(lua_State* state)
{
	char const* filename = lua_tostring(state, 1);
	char const* glyphs = lua_tostring(state, 2);
    float glyph_width = luaL_optnumber(state, 3, 0.0f);
    float glyph_height = luaL_optnumber(state, 4, 0.0f);

	graphics_BitmapFont* bm_font = lua_newuserdata(state, sizeof(graphics_BitmapFont));
	graphics_BitmapFont_new(bm_font, filename, glyphs, glyph_width, glyph_height);

    moduleData.currentFont = bm_font;

	lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.mt);
	lua_setmetatable(state, -2);
    return 1;
}

static int l_graphics_Bitmapfont_setXOffset(lua_State* state)
{
    float offset = lua_tonumber(state, 2);
    char const* glyph = lua_tostring(state, 3);
    graphics_BitmapFont_setGlyphOffsetX(offset, glyph);

    return 0;
}


static int l_graphics_Bitmapfont_setYOffset(lua_State* state)
{
    float offset = lua_tonumber(state, 2);
    char const* glyph = lua_tostring(state, 3);

    graphics_BitmapFont_setGlyphOffsetY(offset, glyph);

    return 0;
}


static int l_graphics_Bitmapfont_setOffset(lua_State* state)
{
    float offsetX = lua_tonumber(state, 2);
    float offsetY = lua_tonumber(state, 3);
    char const* glyph = lua_tostring(state, 4);

    graphics_BitmapFont_setGlyphOffsetX(offsetX, glyph);
    graphics_BitmapFont_setGlyphOffsetY(offsetY, glyph);

    return 0;
}

static int l_graphics_Bitmapfont_setFont(lua_State* state)
{
    graphics_BitmapFont* newFont = l_graphics_toBitmapFont(state, 1);

    if (moduleData.currentFont)
        luaL_unref(state, LUA_REGISTRYINDEX, moduleData.currentFontRef);

    moduleData.currentFontRef = luaL_ref(state, LUA_REGISTRYINDEX);
    moduleData.currentFont = newFont;

    return 0;
}

static int l_graphics_Bitmapfont_getFilter(lua_State* state)
{
    graphics_BitmapFont* font = l_graphics_toBitmapFont(state, 1);

    graphics_Filter filter;

    graphics_Image_getFilter(font->image, &filter);

    l_tools_pushEnum(state, filter.minMode, l_graphics_FilterMode);
    l_tools_pushEnum(state, filter.magMode, l_graphics_FilterMode);
    lua_pushnumber(state, filter.maxAnisotropy);

    return 3;
}

static int l_graphics_Bitmapfont_setFilter(lua_State* state)
{

    graphics_BitmapFont* font = l_graphics_toBitmapFont(state, 1);
    graphics_Filter newFilter;

    graphics_Image_getFilter(font->image, &newFilter);

    newFilter.minMode = l_tools_toEnumOrError(state, 2, l_graphics_FilterMode);
    newFilter.magMode = l_tools_toEnumOrError(state, 3, l_graphics_FilterMode);
    newFilter.maxAnisotropy = luaL_optnumber(state, 4, 1.0f);
    graphics_Image_setFilter(font->image, &newFilter);

    return 0;
}


static int l_graphics_Bitmapfont_print(lua_State* state)
{
	const char* text = lua_tostring(state, 1);
	if (!text)
		text = "";
	int x = luaL_optnumber(state, 2, 0);
	int y = luaL_optnumber(state, 3, x);

	float r = luaL_optnumber(state, 4, 0);
	float sx = luaL_optnumber(state, 5, 1.0f);
	float sy = luaL_optnumber(state, 6, 1.0f);
	float ox = luaL_optnumber(state, 7, 0.0f);
	float oy = luaL_optnumber(state, 8, 0.0f);
	float kx = luaL_optnumber(state, 9, 0.0f);
	float ky = luaL_optnumber(state, 10, 0.0f);

    if (moduleData.currentFont)
        graphics_BitmapFont_render(moduleData.currentFont, text, x, y, r, sx, sy, ox, oy, kx, ky);

	return 0;
}

static int l_graphics_Bitmapfont_gcBitmapfont(lua_State* state)
{
	graphics_BitmapFont* font = l_graphics_toBitmapFont(state, 1);
    graphics_BitmapFont_free(font);
    return 1;
}

static luaL_Reg const bm_fontMetatableFuncs[] = {
	{"__gc", l_graphics_Bitmapfont_gcBitmapfont},
    {"setOffsetX", l_graphics_Bitmapfont_setXOffset},
    {"setOffsetY", l_graphics_Bitmapfont_setYOffset},
    {"setOffset", l_graphics_Bitmapfont_setYOffset},
    {"setFilter", l_graphics_Bitmapfont_setFilter},
    {"getFilter", l_graphics_Bitmapfont_getFilter},
    {NULL, NULL},
};

static luaL_Reg const bm_fontFreeFuncs[] = {
	{"newImageFont", l_graphics_newBitmapFont},
    {"setImageFont", l_graphics_Bitmapfont_setFont},
    {"printb",  l_graphics_Bitmapfont_print},
	{NULL, NULL}
};

void l_graphics_bitmapfont_register(lua_State* state)
{
	l_tools_registerFuncsInModule(state, "graphics", bm_fontFreeFuncs);
	moduleData.mt = l_tools_makeTypeMetatable(state, bm_fontMetatableFuncs);
    moduleData.currentFont = NULL;

	lua_newtable(state);
	lua_newtable(state);
	lua_pushstring(state, "__mode");
	lua_pushstring(state, "v");
	lua_rawset(state, -3);
	lua_setmetatable(state, -2);
	moduleData.ref = luaL_ref(state, LUA_REGISTRYINDEX);

}

l_checkTypeFn(l_graphics_isBitmapFont, moduleData.mt)
l_toTypeFn(l_graphics_toBitmapFont, graphics_BitmapFont)



