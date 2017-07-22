/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include <stdio.h>

#include "../3rdparty/lua/lauxlib.h"
#include "../3rdparty/lua/lua.h"
#include "../3rdparty/lua/lualib.h"

#include "../filesystem/filesystem.h"

#include "filesystem.h"
#include "tools.h"

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

static int l_filesystem_require(lua_State* state) {

    const char* file = l_tools_toStringOrError(state, 1);
    int args = luaL_optinteger(state, 2, 0);
    int returns = luaL_optinteger(state, 3, 0);

    #ifndef CLOVE_TAR
    int err = luaL_loadfile(state, file);
    if (err == LUA_ERRSYNTAX)
        l_tools_trowError(state, lua_tostring(state, -1));

    lua_pcall(state, args, returns, 0);
    #endif
    return 1;
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

static int l_filesystem_equals(lua_State* state)
{
  const char* a = l_tools_toStringOrError(state, 1);
  const char* b = l_tools_toStringOrError(state, 2);
  int l = luaL_optinteger(state, 3, -1);

  if ( filesystem_equals(a, b, l))
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

static int l_filesystem_lines(lua_State* state)
{
	const char* filename = l_tools_toStringOrError(state, 1);
	//lua_pushstring(state, filesystem_line(filename));

	FILE* f;
	f = fopen(filename, "r");
	if (f == NULL)
	{
		printf("%s %s \n", "Could not open: ", filename);
		fclose(f);
		l_tools_trowError(state, "Error in lines");
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);

	int index = 1;
	char line[size+1];

	lua_newtable(state);

	while(fgets(line, sizeof(line), f))
	{
		lua_pushnumber(state, index);
		lua_pushstring(state, line);
		lua_settable(state, -3);
		index++;
	}
	fclose(f);

	return 1;
}

static int l_filesystem_mount(lua_State* state)
{

	const char* path = l_tools_toStringOrError(state, 1);
	const char* mountPoint = l_tools_toStringOrError(state, 2);
	int append = luaL_optinteger(state, 3, 1);

	return filesystem_mount(path, mountPoint, append);
}

static int l_filesystem_unmount(lua_State* state)
{
	const char* path = l_tools_toStringOrError(state, 1);
	lua_pushboolean(state, filesystem_unmount(path));
	return 1;
}

static int l_filesystem_setSource(lua_State* state)
{

	const char* source = l_tools_toStringOrError(state, 1);
	const char* dir = luaL_optstring(state, 2, ".");

	filesystem_setSource(source, dir);
	return 0;
}


static int l_filesystem_getUsrDir(lua_State* state)
{
	lua_pushstring(state, filesystem_getUsrDir());
	return 1;
}

static int l_filesystem_enumerate(lua_State* state)
{
	const char* path = l_tools_toStringOrError(state, 1);

	size_t len = strlen(filesystem_enumerate(path)[0]);

	lua_newtable(state);
	int index = 1;
	for (int i = 0; i < len; i++)
	{
		lua_pushinteger(state, index);
		lua_pushstring(state, filesystem_enumerate(path)[i]);
		lua_settable(state, -3);

		index++;
	}

	return 1;
}

static int l_filesystem_mkDir(lua_State* state)
{
	const char* path = l_tools_toStringOrError(state, 1);

	lua_pushboolean(state, filesystem_mkDir(path));
	return 1;
}

static int l_filesystem_isDir(lua_State* state)
{
	const char* path = l_tools_toStringOrError(state, 1);

	lua_pushboolean(state, filesystem_isDir(path));
	return 1;
}

static int l_filesystem_setIdentity(lua_State* state)
{
	const char* path = l_tools_toStringOrError(state, 1);

	lua_pushboolean(state, filesystem_setIdentity(path));
	return 1;
}

static luaL_Reg const regFuncs[] = {
  {"setIdentity", l_filesystem_setIdentity},
  {"unmount", l_filesystem_unmount},
  {"mount", l_filesystem_mount},
  {"setSource", l_filesystem_setSource},
  {"getUsrDir", l_filesystem_getUsrDir},
  {"enumerate", l_filesystem_enumerate},
  {"mkDir", l_filesystem_mkDir},
  {"isDir", l_filesystem_isDir},
  {"load", l_filesystem_load},
  {"lines", l_filesystem_lines},
  {"require", l_filesystem_require},
  {"isFile", l_filesystem_isFile},
  {"getCurrentDirectory", l_filesystem_getCurrentDirectory},
  {"getSaveDirectory", l_filesystem_getSaveDirectory},
  {"getSource", l_filesystem_getSource},
  {"remove", l_filesystem_remove},
  {"read", l_filesystem_read},
  {"exists", l_filesystem_exists},
  {"write", l_filesystem_write},
  {"append", l_filesystem_append},
  {"equals", l_filesystem_equals},
  {"contain", l_filesystem_contain},
  {NULL, NULL}
};

int l_filesystem_register(lua_State* state) {
  l_tools_registerModule(state, "filesystem", regFuncs);
  return 0;
}
