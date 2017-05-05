/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once


#include "3rdparty/lua/lua.h"
#include "3rdparty/lua/lauxlib.h"
#include "3rdparty/lua/lualib.h"
#include "3rdparty/SDL2/include/SDL.h"
#include "3rdparty/microtar/microtar.h"

#include "luaapi/audio.h"
#include "luaapi/mouse.h"
#include "luaapi/event.h"
#include "luaapi/graphics.h"
#include "luaapi/graphics_font.h"
#include "luaapi/image.h"
#include "luaapi/love.h"
#include "luaapi/boot.h"
#include "luaapi/joystick.h"
#include "luaapi/keyboard.h"
#include "luaapi/mouse.h"
#include "luaapi/filesystem.h"
#include "luaapi/timer.h"
#include "luaapi/math.h"
#include "luaapi/system.h"
#include "luaapi/thread.h"
#include "luaapi/errorhandler.h"

#include "tools/utils.h"
#include "graphics/graphics.h"
#include "graphics/matrixstack.h"
#include "filesystem/filesystem.h"
#include "audio/audio.h"
#include "audio/streamsource.h"
#include "timer/timer.h"

#include "love.h"
#include "joystick.h"
#include "keyboard.h"
#include "mouse.h"

	/* Only if USE_NATIVE is declared in tools/utils.c
	 * then use it */
#ifdef USE_NATIVE
	#include "../native/game.h"
#endif

void main_activity_load(int argc, char* argv[]);
void main_loop(void *data);

