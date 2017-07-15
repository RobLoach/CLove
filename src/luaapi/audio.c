/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "tools.h"
#include "audio.h"
#include "../3rdparty/lua/lua.h"

static struct {
	int audioDataMT;
	audio_Type audio_type;
} moduleData;

static int l_audio_newSource(lua_State *state) {
	char const* filename = l_tools_toStringOrError(state, 1);
	const char* type = luaL_optstring(state, 2, "static");
	int err;

	if (strcmp(type,"stream") == 0) {
		audio_StreamSource *src = lua_newuserdata(state, sizeof(audio_StreamSource));
		err = audio_loadStream(src, filename);
		moduleData.audio_type = audio_type_stream;
	} else if (strcmp(type, "static") == 0) {
		audio_StaticSource *src = lua_newuserdata(state, sizeof(audio_StaticSource));
		err = audio_loadStatic(src, filename);
		moduleData.audio_type = audio_type_static;
	}

	lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.audioDataMT);
	if(err == -1){
		lua_pushstring(state, "Could not load sound file: ");
		lua_pushstring(state, lua_tostring(state, 1));
		lua_pushstring(state, ", reason: unknow file type");
		lua_concat(state, 3);
		return lua_error(state);
	}
	else if(err == 0){
		lua_pushstring(state, "Could not load sound file: ");
		lua_pushstring(state, lua_tostring(state, 1));
		lua_pushstring(state, ", reason: file does not exist");
		lua_concat(state, 3);
		return lua_error(state);
	}

	lua_setmetatable(state, -2);
	return 1;
}

static int l_audio_SourceCommon_getType(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream)
		lua_pushstring(state, "stream");
	if (moduleData.audio_type == audio_type_static)
		lua_pushstring(state, "static");
	return 1;
}

static int l_audio_SourceCommon_play(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_play(source);
	}
	else
	{
		audio_StaticSource* source = (audio_StaticSource *)lua_touserdata(state, 1);
		audio_StaticSource_play(source);
	}

	return 0;
}

static int l_audio_SourceCommon_stop(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_stop(source);
	} else {
		audio_StaticSource* source = (audio_StaticSource *)lua_touserdata(state, 1);
		audio_StaticSource_stop(source);
	}
	return 0;
}

static int l_audio_SourceCommon_pause(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_pause(source);
	} else {
		audio_StaticSource* source = (audio_StaticSource *)lua_touserdata(state, 1);
		audio_StaticSource_pause(source);
	}
	return 0;
}

static int l_audio_SourceCommon_resume(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_resume(source);
	} else {
		audio_StaticSource* source = (audio_StaticSource *)lua_touserdata(state, 1);
		audio_StaticSource_resume(source);
	}
	return 1;
}

static int l_audio_SourceCommon_isStopped(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		lua_pushboolean(state, audio_StreamSource_isStopped(source));
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		lua_pushboolean(state, audio_SourceCommon_isStopped(source));
	}
	return 1;
}

static int l_audio_SourceCommon_isPlaying(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		lua_pushboolean(state, audio_StreamSource_isPlaying(source));
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		lua_pushboolean(state, audio_SourceCommon_isPlaying(source));
	}
	return 1;
}

static int l_audio_SourceCommon_isPaused(lua_State* state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		lua_pushboolean(state, audio_StreamSource_isPaused(source));
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		lua_pushboolean(state, audio_SourceCommon_isPaused(source));
	}
	return 1;
}

static int l_audio_SourceCommon_setVolume(lua_State *state) {
	float gain = l_tools_toNumberOrError(state, 2);
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_setVolume(source, gain);
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		audio_SourceCommon_setVolume(source, gain);
	}
	return 1;
}

static int l_audio_SourceCommon_getVolume(lua_State *state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		lua_pushnumber(state, audio_StreamSource_getVolume(source));
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		lua_pushnumber(state, audio_SourceCommon_getVolume(source));
	}
	return 1;
}

static int l_audio_setVolumeGlobal(lua_State* state){
	double volume = lua_tonumber(state, 1);
	if(volume <= 0.0f)
		volume = 0.0f;
	else if (volume >= 1.0f)
		volume = 1.0f;

	audio_setVolume(volume);
	return 1;
}

static int l_audio_SourceCommon_setPitch(lua_State *state) {
	float pitch = l_tools_toNumberOrError(state, 2);
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_setPitch(source, pitch);
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		audio_SourceCommon_setPitch(source, pitch);
	}
	return 1;
}

static int l_audio_SourceCommon_getPitch(lua_State *state) {
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		lua_pushnumber(state, audio_StreamSource_getPitch(source));
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		lua_pushnumber(state, audio_SourceCommon_getPitch(source));
	}
	return 1;
}

static int l_audio_SourceCommon_setPosition(lua_State* state){
	float x = lua_tonumber(state, 2);
	float y = lua_tonumber(state, 3);
	float z = lua_tonumber(state, 4);
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_setPosition(source, x, y, z);
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		audio_SourceCommon_setPosition(source, x, y, z);
	}
	return 1;
}

static int l_audio_SourceCommon_setVelocity(lua_State* state){
	float x = lua_tonumber(state, 2);
	float y = lua_tonumber(state, 3);
	float z = lua_tonumber(state, 4);
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_setVelocity(source, x, y, z);
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		audio_SourceCommon_setVelocity(source, x, y, z);
	}
	return 1;
}

static int l_audio_SourceCommon_setLooping(lua_State* state){
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_setLooping(source, lua_toboolean(state, 2));
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		audio_SourceCommon_setLooping(source, lua_toboolean(state, 2));
	}
	return 1;
}

static int l_audio_SourceCommon_free(lua_State* state){
	if (moduleData.audio_type == audio_type_stream){
		audio_StreamSource* source = (audio_StreamSource *)lua_touserdata(state, 1);
		audio_StreamSource_free(source, lua_toboolean(state, 2));
	} else {
		audio_SourceCommon* source = (audio_SourceCommon*)lua_touserdata(state, 1);
		audio_SourceCommon_free(source);
	}
	return 1;
}

static luaL_Reg const SourceMetatableFuncs[] = {
	{"__gc", l_audio_SourceCommon_free},
	{"getType", l_audio_SourceCommon_getType},
	{"setLooping", l_audio_SourceCommon_setLooping},
	{"setVelocity", l_audio_SourceCommon_setVelocity},
	{"setPitch",   l_audio_SourceCommon_setPitch},
	{"getPitch",   l_audio_SourceCommon_getPitch},
	{"setPosition",   l_audio_SourceCommon_setPosition},
	{"play",       l_audio_SourceCommon_play},
	{"setVolume",  l_audio_SourceCommon_setVolume},
	{"getVolume",  l_audio_SourceCommon_getVolume},
	{"isPlaying",  l_audio_SourceCommon_isPlaying},
	{"isPaused",  l_audio_SourceCommon_isPaused},
	{"isStopped",  l_audio_SourceCommon_isStopped},
	{"pause",  l_audio_SourceCommon_pause},
	{"resume",  l_audio_SourceCommon_resume},
	{"stop",  l_audio_SourceCommon_stop},
	{NULL, NULL}
};

static luaL_Reg const regFuncs[] = {
	{"newSource", l_audio_newSource},
	{"setVolume", l_audio_setVolumeGlobal},
	{NULL, NULL}
};

int l_audio_register(lua_State *state) {
	l_tools_registerModule(state, "audio", regFuncs);
	moduleData.audioDataMT = l_tools_makeTypeMetatable(state, SourceMetatableFuncs);
	return 1;
}
