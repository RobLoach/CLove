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

#include "../3rdparty/lua/lua.h"

#include "../graphics/font.h"

void l_graphics_font_register(lua_State* state);
int l_graphics_newFont(lua_State* state);

bool l_graphics_isFont(lua_State* state, int index);
graphics_Font* l_graphics_toFont(lua_State* state, int index);
