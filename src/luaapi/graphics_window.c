/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../3rdparty/lua/lauxlib.h"

#include "../graphics/graphics.h"
#include "graphics_window.h"
#include "tools.h"

static int l_graphics_window_getWidth(lua_State* state) {
  lua_pushnumber(state, graphics_getWidth());
  return 1;
}

static int l_graphics_window_getHeight(lua_State* state) {
  lua_pushnumber(state, graphics_getHeight());
  return 1;
}

static int l_graphics_window_setTitle(lua_State* state){
  graphics_setTitle(lua_tostring(state, 1));
  return 1;
}

static int l_graphics_window_setMode(lua_State* state){
  graphics_setMode(l_tools_toNumberOrError(state, 1), l_tools_toNumberOrError(state, 2));
  return 1;
}

static int l_graphics_window_setFullscreen(lua_State* state){
  graphics_setFullscreen(l_tools_toBooleanOrError(state, 1), lua_tostring(state, 2));
  return 1;
}

static int l_graphics_window_isCreated(lua_State* state){
  lua_pushboolean(state,graphics_isCreated());
  return 1;
}

static int l_graphics_window_getTitle(lua_State* state){
  lua_pushstring(state, graphics_getTitle());
  return 1;
}

static int l_graphics_window_setPosition(lua_State* state){
  graphics_setPosition(l_tools_toNumberOrError(state, 1), lua_tointeger(state, 2));
  return 1;
}

static int l_graphics_window_hasFocus(lua_State* state){
  lua_pushinteger(state, graphics_hasFocus());
  return 1;
}

static int l_graphics_window_hasMouseFocus(lua_State* state){
  lua_pushinteger(state, graphics_hasMouseFocus());
  return 1;
}

static int l_graphics_window_getDimensions(lua_State* state) {
  lua_pushnumber(state, graphics_getWidth());
  lua_pushnumber(state, graphics_getHeight());
  return 2;
}

static luaL_Reg const windowFreeFuncs[] = {
  {"setFullscreen", l_graphics_window_setFullscreen},
  {"getDimensions", l_graphics_window_getDimensions},
  {"setMode", l_graphics_window_setMode},
  {"isCreated", l_graphics_window_isCreated},
  {"setTitle", l_graphics_window_setTitle},
  {"getTitle", l_graphics_window_getTitle},
  {"setPosition", l_graphics_window_setPosition},
  {"getWidth", l_graphics_window_getWidth},
  {"getHeight", l_graphics_window_getHeight},
  {"hasFocus",  l_graphics_window_hasFocus},
  {"hasMouseFocus",  l_graphics_window_hasMouseFocus},
  {NULL, NULL}
};

void l_graphics_window_register(lua_State* state) {
  l_tools_registerModule(state, "window", windowFreeFuncs);
}

