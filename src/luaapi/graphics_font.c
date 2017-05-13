/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../3rdparty/lua/lauxlib.h"
#include "graphics_font.h"

#include "tools.h"
#include "graphics.h"

#include "../graphics/font.h"
#include "../graphics/matrixstack.h"
#include "../graphics/shader.h"
#include "../graphics/graphics.h"

static struct {
	int fontMT;
	graphics_Font* currentFont;
	int currentFontRef;
	int loadedFontsRef;
	graphics_Font defaultFont;
} moduleData;


int l_graphics_newFont(lua_State* state) {

	char const * filename = NULL;
	int ptsize = 12;

	const int type = lua_type(state, 1);
	int err = lua_isnil(state, 1);

	if (err == 0) {
		if(type == LUA_TSTRING) {
			filename = lua_tostring(state, 1);
			if(lua_isnumber(state, 2)) {
				ptsize = lua_tonumber(state, 2);
			}
			else {
				ptsize = 12;
				lua_settop(state, 1);
				lua_pushnumber(state, ptsize);
			}
			lua_settop(state, 2);
		} else if(type == LUA_TNUMBER) {
			ptsize = l_tools_toNumberOrError(state, 1);
			lua_settop(state, 1);
			lua_pushstring(state, "(default)");
			lua_insert(state, 1);
		}
	} else if (err == 1) {
		ptsize = 12;
		lua_settop(state, 1);
		lua_pushnumber(state, ptsize);
	}


	graphics_Font* font = lua_newuserdata(state, sizeof(graphics_Font));
	graphics_Font_new(font, filename, ptsize);
	// Stack: ... fonts fontname raw-font metatable
	lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.fontMT);

	// Stack: ... fonts fontname constructed-font
	lua_setmetatable(state, -2);
	return 1;
}

static void l_graphics_loadDefaultFont() {
	graphics_Font_new(&moduleData.defaultFont, NULL, 12);
	moduleData.currentFont = &moduleData.defaultFont;
}

static int l_graphics_setFont(lua_State* state) {
	lua_settop(state, 1);

	graphics_Font* font;

	if (lua_isnumber(state,1)) {
		graphics_Font_new(&moduleData.defaultFont, NULL, lua_tonumber(state,1));
		font = &moduleData.defaultFont;
	} else {
		font = l_graphics_toFont(state, 1);
	}
	// Release current font in Lua, so it can be GCed if needed
	if(moduleData.currentFont) {
		luaL_unref(state, LUA_REGISTRYINDEX, moduleData.currentFontRef);
	}

	moduleData.currentFontRef = luaL_ref(state, LUA_REGISTRYINDEX);
	moduleData.currentFont = font;
	return 0;
}


static int l_graphics_getFont(lua_State* state) {
	lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.currentFontRef);
	return 1;
}


static const l_tools_Enum l_graphics_AlignMode[] = {
	{"left", graphics_TextAlign_left},
	{"right", graphics_TextAlign_right},
	{"center", graphics_TextAlign_center},
	{"justify", graphics_TextAlign_justify},
	{NULL, 0}
};

static int l_graphics_printf(lua_State* state) {
	if(!moduleData.currentFont) {
		l_graphics_loadDefaultFont();
	}

	char const* text = lua_tostring(state, 1);
	if(!text) {
		text = "";
	}
	int x = luaL_optnumber(state, 2, 0);
	int y = luaL_optnumber(state, 3, x);
	int limit = l_tools_toNumberOrError(state, 4);
	graphics_TextAlign align = graphics_TextAlign_left;
	if(!lua_isnoneornil(state, 5)) {
		align = l_tools_toEnumOrError(state, 5, l_graphics_AlignMode);
	}

	float r = luaL_optnumber(state, 6, 0);
	float sx = luaL_optnumber(state, 7, 1.0f);
	float sy = luaL_optnumber(state, 8, sx);
	float ox = luaL_optnumber(state, 9, 0);
	float oy = luaL_optnumber(state, 10, 0);
	float kx = luaL_optnumber(state, 11, 0);
	float ky = luaL_optnumber(state, 12, 0);

	//graphics_Font_printf(moduleData.currentFont, text, x, y, limit, align, r, sx, sy, ox, oy, kx, ky);
	//graphics_Font_render(moduleData.currentFont, text, x, y, r, sx, sy, ox, oy, kx, ky);
	return 0;
}

static int l_graphics_print(lua_State* state) {
	if(!moduleData.currentFont) {
		l_graphics_loadDefaultFont();
	}
	char const* text = lua_tostring(state, 1);
	if(!text) {
		text = "";
	}
	int x = luaL_optnumber(state, 2, 0);
	int y = luaL_optnumber(state, 3, x);

	float r = luaL_optnumber(state, 4, 0);
	float sx = luaL_optnumber(state, 5, 1.0f);
	float sy = luaL_optnumber(state, 6, sx);
	float ox = luaL_optnumber(state, 7, 0);
	float oy = luaL_optnumber(state, 8, 0);
	float kx = luaL_optnumber(state, 9, 0);
	float ky = luaL_optnumber(state, 10, 0);

	//graphics_Font_print(moduleData.currentFont, text, x, y, r, sx, sy, ox, oy, kx, ky);
	graphics_Font_render(moduleData.currentFont, text, x, y, r, sx, sy, ox, oy, kx, ky);
	return 0;
}

static int l_graphics_gcFont(lua_State* state) {
	graphics_Font* font = l_graphics_toFont(state, 1);
	graphics_Font_free(font);
	return 1;
}

static int l_graphics_Font_getHeight(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);

	//char const* line = l_tools_toStringOrError(state, 2);
	int height = graphics_Font_getHeight(font);

	lua_pushinteger(state, height);
	return 1;
}

static int l_graphics_Font_getWidth(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);

	char const* line = l_tools_toStringOrError(state, 2);
	int width = graphics_Font_getWidth(font, line);

	lua_pushinteger(state, width);
	return 1;
}

static int l_graphics_Font_getDescent(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);

	lua_pushinteger(state, graphics_Font_getDescent(font));
	return 1;
}

static int l_graphics_Font_getAscent(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);

	lua_pushinteger(state, graphics_Font_getAscent(font));
	return 1;
}

static int l_graphics_Font_getBaseline(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);

	lua_pushinteger(state, graphics_Font_getBaseline(font));
	return 1;
}

static int l_graphics_Font_getWrap(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);

	char const* line = l_tools_toStringOrError(state, 2);
	int width = l_tools_toNumberOrError(state, 3);

    lua_pushstring(state, "not yet implemented!");
    lua_pushinteger(state, graphics_Font_getWrap(font, line, width, NULL));
    return 2;
}

static int l_graphics_Font_getFilter(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);

	graphics_Filter filter;

	graphics_Font_getFilter(font, &filter);

	l_tools_pushEnum(state, filter.minMode, l_graphics_FilterMode);
	l_tools_pushEnum(state, filter.magMode, l_graphics_FilterMode);
	lua_pushnumber(state, filter.maxAnisotropy);

	return 3;
}

static int l_graphics_Font_setFilter(lua_State* state) {
	l_assertType(state, 1, l_graphics_isFont);

	graphics_Font* font = l_graphics_toFont(state, 1);
	graphics_Filter newFilter;
	graphics_Font_getFilter(font, &newFilter);
	newFilter.minMode = l_tools_toEnumOrError(state, 2, l_graphics_FilterMode);
	newFilter.magMode = l_tools_toEnumOrError(state, 3, l_graphics_FilterMode);
	newFilter.maxAnisotropy = luaL_optnumber(state, 4, 1.0f);
	graphics_Font_setFilter(font, &newFilter);

	return 0;
}

static luaL_Reg const fontMetatableFuncs[] = {
	{"__gc",               l_graphics_gcFont},
	{"getHeight",          l_graphics_Font_getHeight},
	{"getAscent",          l_graphics_Font_getAscent},
	{"getDescent",         l_graphics_Font_getDescent},
	{"getBaseline",        l_graphics_Font_getBaseline},
	{"getWidth",           l_graphics_Font_getWidth},
	{"getWrap",            l_graphics_Font_getWrap},
	{"getFilter",          l_graphics_Font_getFilter},
	{"setFilter",          l_graphics_Font_setFilter},
	{NULL, NULL}
};

static luaL_Reg const fontFreeFuncs[] = {
	{"newFont",            l_graphics_newFont},
	{"setFont",            l_graphics_setFont},
	{"getFont",            l_graphics_getFont},
	{"printf",             l_graphics_printf},
	{"print",              l_graphics_print},
	{NULL, NULL}
};

void l_graphics_font_register(lua_State* state) {
    l_tools_registerFuncsInModule(state, "graphics", fontFreeFuncs);
    moduleData.fontMT   = l_tools_makeTypeMetatable(state, fontMetatableFuncs);
    moduleData.currentFont = NULL;

    lua_newtable(state);
    lua_newtable(state);
    lua_pushstring(state, "__mode");
    lua_pushstring(state, "v");
    lua_rawset(state, -3);
    lua_setmetatable(state, -2);
    moduleData.loadedFontsRef = luaL_ref(state, LUA_REGISTRYINDEX);

}

l_checkTypeFn(l_graphics_isFont, moduleData.fontMT)
l_toTypeFn(l_graphics_toFont, graphics_Font)
