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
	graphics_BitmapFont* currentFont;
} moduleData;

int l_graphics_newBitmapFont(lua_State* state)
{
	char const* filename = lua_tostring(state, 1);
	char const* glyphs = lua_tostring(state, 2);
	float glyph_width = lua_tonumber(state, 3);
	float glyph_height = lua_tonumber(state, 4);

	graphics_BitmapFont* bm_font = lua_newuserdata(state, sizeof(graphics_BitmapFont));
	graphics_BitmapFont_new(bm_font, filename, glyphs, glyph_width, glyph_height);

	lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.mt);
	lua_setmetatable(state, -2);

	moduleData.currentFont = bm_font;

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

	graphics_BitmapFont_render(moduleData.currentFont, text, x, y, r, sx, sy, ox, oy, kx, ky);

	return 0;
}

static int l_graphics_Bitmapfont_gcBitmapfont(lua_State* state)
{
	graphics_BitmapFont* font = l_graphics_toBitmapFont(state, 1);
	graphics_BitmapFont_free(font);
	return 0;
}

static luaL_Reg const bm_fontMetatableFuncs[] = {
	{"__gc", l_graphics_Bitmapfont_gcBitmapfont},
	{NULL, NULL},
};

static luaL_Reg const bm_fontFreeFuncs[] = {
	{"newImageFont", l_graphics_newBitmapFont},
	{"printBitmap",  l_graphics_Bitmapfont_print},
	{NULL, NULL}
};

void l_graphics_bitmapfont_register(lua_State* state)
{
	l_tools_registerFuncsInModule(state, "graphics", bm_fontFreeFuncs);
	moduleData.mt = l_tools_makeTypeMetatable(state, bm_fontMetatableFuncs);

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



