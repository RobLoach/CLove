/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_geometry.h"
#include "../graphics/geometry.h"
#include "tools.h"
#include "../tools/log.c"

static int l_geometry_circle(lua_State* state) {
  const char* type = l_tools_toStringOrError(state, 1);
  float x = l_tools_toNumberOrError(state, 2);
  float y = l_tools_toNumberOrError(state, 3);
  float radius = l_tools_toNumberOrError(state, 4);
  float segments = l_tools_toNumberOrError(state, 5);

  if (strncmp(type,"line",4) == 0)
    graphics_geometry_drawCircle(x, y, radius, segments);
  else if(strncmp(type,"fill", 4) == 0)
    graphics_geometry_fillCircle(x, y, radius, segments);

  return 1;
}

static int l_geometry_rectangle(lua_State* state) {
  const char* type = l_tools_toStringOrError(state, 1);
  float x = l_tools_toNumberOrError(state, 2);
  float y = l_tools_toNumberOrError(state, 3);
  float w = l_tools_toNumberOrError(state, 4);
  float h = l_tools_toNumberOrError(state, 5);

  if (strncmp(type,"line",4) == 0)
    graphics_geometry_drawRectangle(x, y, w, h);
  else if(strncmp(type,"fill", 4) == 0)
    graphics_geometry_fillRectangle(x, y, w, h);

  return 1;
}

static int l_geometry_points(lua_State* state) {
  float x = l_tools_toNumberOrError(state, 1);
  float y = l_tools_toNumberOrError(state, 2);

  graphics_geometry_points (x, y);

  return 1;
}


static int l_geometry_setLineWidth(lua_State* state) {
  float width = l_tools_toNumberOrError(state, 1);
  //graphics_geometry_setLineWidth(width);
  return 1;
}

static int l_geometry_getLineWidth(lua_State* state) {
  //lua_pushnumber(state, graphics_geometry_getLineWidth());
  return 1;
}

static luaL_Reg const geometryFuncs[] = {
  {"points",       l_geometry_points},
  {"point",       l_geometry_points},
  {"rectangle",    l_geometry_rectangle},
  {"circle",       l_geometry_circle},
  {"setLineWidth", l_geometry_setLineWidth},
  {"getLineWidth", l_geometry_getLineWidth},
  {NULL,NULL}
};

void l_graphics_geometry_register(lua_State* state) {
  l_tools_registerFuncsInModule(state, "graphics", geometryFuncs);
}
