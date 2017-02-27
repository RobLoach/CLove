/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include "../3rdparty/lua/lauxlib.h"
#include "../3rdparty/lua/lua.h"
#include "../3rdparty/lua/lualib.h"
#include "filesystem.h"
#include "tools.h"
#include "../filesystem/filesystem.h"

static int l_filesystem_read(lua_State* state) {
  // TODO implement max length
  char const* filename = l_tools_toStringOrError(state, 1);
  char* data = NULL;
  int len = filesystem_read(filename, &data);
  if(len < 0) {
      lua_pushstring(state, "could not read file");
      return lua_error(state);
    }

  lua_pushstring(state, data);
  free(data);
  lua_pushnumber(state, len);
  return 2;
}

static int l_filesystem_getSaveDirectory(lua_State* state) {
    const char* company = luaL_optstring(state, 2, "Clove");
    const char* projName = luaL_optstring(state, 1, "myGame");
    const char* path = filesystem_getSaveDirectory(company, projName);
    lua_pushstring(state, path);
    return 1;
}

static int l_filesystem_exists(lua_State* state)
{
  const char* filename = l_tools_toStringOrError(state, 1);
  return filesystem_exists(filename);
}

static int l_filesystem_write(lua_State* state)
{
  const char* filename = l_tools_toStringOrError(state, 1);
  const char* data = l_tools_toStringOrError(state, 2);
  filesystem_write(filename, data);
  return 2;
}

static int l_filesystem_append(lua_State* state)
{
  const char* filename = l_tools_toStringOrError(state, 1);
  const char* data = l_tools_toStringOrError(state, 2);
  filesystem_append(filename, data);
  return 2;
}

static int l_filesystem_getSource(lua_State* state) {
  lua_pushstring(state, filesystem_getSource());
  return 1;
}
static int l_filesystem_load(lua_State* state) {
  char const* filename = l_tools_toStringOrError(state, 1);
  char* data = NULL;
  int len = filesystem_read(filename, &data);
  if(len < 0) {
      lua_pushstring(state, "could not read file");
      return lua_error(state);
    }

  luaL_loadstring(state, data);
  free(data);
  return 1;
}

static int l_filesystem_remove(lua_State* state) {
  char const* file = l_tools_toStringOrError(state, 1);
  filesystem_remove(file);
  return 0;
}

static int l_filesystem_contain(lua_State* state)  {
  const char* a = l_tools_toStringOrError(state, 1);
  const char* b = l_tools_toStringOrError(state, 2);
  if ( filesystem_contain(a, b))
    lua_pushboolean(state, 1);
  else
    lua_pushboolean(state, 0);
  return 1;
}

static int l_filesystem_compare(lua_State* state)  {
  const char* a = l_tools_toStringOrError(state, 1);
  const char* b = l_tools_toStringOrError(state, 2);
  int l = l_tools_toNumberOrError(state, 3);
  if ( filesystem_compare(a, b, l))
    lua_pushboolean(state, 1);
  else
    lua_pushboolean(state, 0);
  return 1;
}

static int l_filesystem_getCurrentDirectory(lua_State* state) {
    lua_pushstring(state, filesystem_getCurrentDirectory());
    return 1;
}

static int l_filesystem_isFile(lua_State* state) {
    const char* file = l_tools_toStringOrError(state, 1);
    const char* string_mode = luaL_optstring(state, 2, "e"); //default check only for existence
    int mode = 0;
    
    if (strncmp(string_mode,"e", 1) == 0)
        mode = 0;
    else if (strncmp(string_mode,"w", 1) == 0)
        mode = 2;
    else if (strncmp(string_mode,"r", 1) == 0)
        mode = 4;
    else if (strncmp(string_mode,"wr", 2) == 0)
        mode = 6;

    int isFile = filesystem_isFile(file, mode); 

    if (isFile == 0)
        lua_pushboolean(state, 1);
    else 
        lua_pushboolean(state, 0);

    return 1;
}

static luaL_Reg const regFuncs[] = {
  {"load", l_filesystem_load},
  {"isFile", l_filesystem_isFile},
  {"getCurrentDirectory", l_filesystem_getCurrentDirectory},
  {"getSaveDirectory", l_filesystem_getSaveDirectory},
  {"getSource", l_filesystem_getSource},
  {"remove", l_filesystem_remove},
  {"read", l_filesystem_read},
  {"exists", l_filesystem_exists},
  {"write", l_filesystem_write},
  {"append", l_filesystem_append},
  {"compare", l_filesystem_compare},
  {"contain", l_filesystem_contain},
  {NULL, NULL}
};

int l_filesystem_register(lua_State* state) {
  l_tools_registerModule(state, "filesystem", regFuncs);
  return 0;
}
