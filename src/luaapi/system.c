/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "system.h"
#include "../system.h"
#include "tools.h"
#include <stdlib.h>
#include <stdio.h>

static int l_system_getOS(lua_State* state) {
  lua_pushstring (state, system_getOS());
 return 1;
}

static int l_system_getPowerInfo(lua_State* state) {
  system_PowerState power = system_getPowerInfo();
  lua_pushstring(state, power.state);
  lua_pushinteger(state, power.seconds);
  lua_pushinteger(state, power.percent);
  return 3;
}

static luaL_Reg const regFuncs[] = {
  {"getOS", l_system_getOS},
  {"getPowerInfo", l_system_getPowerInfo},
  {NULL,NULL}
};

int l_system_register(lua_State* state) {
  l_tools_registerModule(state, "system", regFuncs);
  return 0;
}
