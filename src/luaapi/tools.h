/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <string.h>
#include <stdbool.h>

#include "../3rdparty/lua/lua.h"
#include "../3rdparty/lua/lauxlib.h"

#include "../love_config.h"
#include "../graphics/font.h"

void l_tools_registerFuncsInModule(lua_State* state, char const* module, luaL_Reg const* funcs);
void l_tools_registerModule(lua_State* state, char const* moduleName, luaL_Reg const * funcs);
int l_tools_makeTypeMetatable(lua_State* state, luaL_Reg const* funcs);

#ifndef LOVE_SKIP_SAFETY_CHECKS

inline void l_tools_trowError(lua_State* state, const char* msg) {
    lua_pushstring(state, msg);
    lua_error(state);
}

inline float l_tools_toNumberOrError(lua_State* state, int index) {
  if(lua_type(state, index) != LUA_TNUMBER) {
      luaL_argerror(state,index-1,"expected number");
      lua_error(state);
    }

  return lua_tonumber(state, index);
}

inline char const* l_tools_toStringOrError(lua_State* state, int index) {
  if(lua_type(state, index) != LUA_TSTRING) {
      luaL_argerror(state,index,"expected string");
      lua_error(state);
    }

  return lua_tostring(state, index);
}

inline bool l_tools_optBoolean(lua_State* state, int index, int def) {
    int get = def;
    if (lua_isnil(state, index))
        get = def;
    else
        get = lua_toboolean(state, index);

    if (get <= 0)
        return false;

    return true;
}

inline int l_tools_toBooleanOrError(lua_State* state, int index) {
  if(lua_type(state, index) != LUA_TBOOLEAN) {
      luaL_argerror(state,index-1,"expected boolean");
      lua_error(state);
    }

  return lua_toboolean(state, index);
}
#else
inline float l_tools_toNumberOrError(lua_State* state, int index) {
  return lua_tonumber(state, index);
}

inline char const* l_tools_toStringOrError(lua_State* state, int index) {
  return lua_tostring(state, index);
}

inline int l_tools_toBooleanOrError(lua_State* state, int index) {
  return lua_toboolean(state, index);
}
#endif

typedef struct {
  char const * name;
  int value;
} l_tools_Enum;

inline int l_tools_toEnumOrError(lua_State* state, int index, l_tools_Enum const* values) {
  char const* string = l_tools_toStringOrError(state, index);

  while(values->name) {
      if(!strcmp(values->name, string)) {
          return values->value;
        }
      ++values;
    }

  luaL_argerror(state,index-1,"invalid enum value");
  return lua_error(state);
}

void l_tools_pushEnum(lua_State* state, int value, l_tools_Enum const* values);

#define l_checkTypeFn(name, typeMT) \
  bool name(lua_State* state, int index) { \
  if(lua_type(state, index) != LUA_TUSERDATA) \
  return 0;                                 \
  lua_getmetatable(state, index);             \
  lua_pushstring(state, "type");              \
  lua_rawget(state, -2);                      \
  bool matching = lua_tointeger(state, -1) == typeMT; \
  lua_pop(state, 2);                          \
  return matching;                          \
  }

#define l_toTypeFn(name, type) \
  type* name(lua_State* state, int index) {\
  return (type*)lua_touserdata(state, index);\
  }

#ifndef LOVE_SKIP_SAFETY_CHECKS
# define l_assertType(state, index, func) \
  if(!func(state, index)) { \
  luaL_argerror(state,index-1,"expected X"); \
  lua_error(state); \
  }
#else
# define l_assertType(state, index, func)
#endif
