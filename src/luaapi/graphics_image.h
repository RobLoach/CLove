/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "../3rdparty/lua/lauxlib.h"
#include "../3rdparty/lua/lualib.h"
#include "../3rdparty/lua/lua.h"

#include "image.h"
#include "tools.h"
#include "graphics.h"
#include "graphics_image.h"

#include "../graphics/image.h"
#include "../graphics/gltools.h"

typedef struct {
    graphics_Image image;
    int imageDataRef;
    const char* path;
} l_graphics_Image;

void l_graphics_image_register(lua_State* state);
bool l_graphics_isImage(lua_State* state, int index);
l_graphics_Image* l_graphics_toImage(lua_State* state, int index);
int l_graphics_newImage(lua_State* state);
