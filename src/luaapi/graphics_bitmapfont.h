/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <stdbool.h>

#include "../3rdparty/lua/lua.h"

#include "../graphics/bitmap_font.h"

void l_graphics_bitmapfont_register(lua_State* state);
int l_graphics_newBitmapFont(lua_State* state);

bool l_graphics_isBitmapFont(lua_State* state, int index);
graphics_BitmapFont* l_graphics_toBitmapFont(lua_State* state, int index);
