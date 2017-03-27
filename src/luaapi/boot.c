/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../3rdparty/lua/lauxlib.h"

#include "boot.h"
#include "tools.h"
#include "../graphics/graphics.h"
#include "../love.h"

static void setConfDefault(lua_State* state, love_Config *config) {

    lua_pushstring(state, "window");
    lua_rawget(state, -2);

    love_Version const* version = love_getVersion();

    lua_pushstring(state, "version");
    lua_rawget(state, -2);
    config->window.version = luaL_optstring(state, -1, version->strVersion);
    lua_pop(state, 1);

    if (strncmp(version->strVersion, config->window.version, 5) != 0)
        printf("Warning: This application is not built for version: %s %s %s \n", version->strVersion, "but:", config->window.version);

    lua_pushstring(state, "width");
    lua_rawget(state, -2);
    config->window.width = luaL_optinteger(state, -1, 800);
    lua_pop(state, 1);

    lua_pushstring(state, "height");
    lua_rawget(state, -2);
    config->window.height = luaL_optinteger(state, -1, 600);
    lua_pop(state, 1);

    lua_pushstring(state, "title");
    lua_rawget(state, -2);
    config->window.title = luaL_optstring(state, -1, "Untitled CLove");
    lua_pop(state, 1);

    lua_pushstring(state, "x");
    lua_rawget(state, -2);
    config->window.x = luaL_optinteger(state, -1, -1);
    lua_pop(state, 1);

    lua_pushstring(state, "y");
    lua_rawget(state, -2);
    config->window.y = luaL_optinteger(state, -1, -1);
    lua_pop(state, 1);

    lua_pushstring(state, "vsync");
    lua_rawget(state, -2);
    config->window.vsync = l_tools_optBoolean(state, -1, 1);
    lua_pop(state, 1);

    lua_pushstring(state, "minheight");
    lua_rawget(state, -2);
    config->window.minheight = luaL_optinteger(state, -1, 1);
    lua_pop(state, 1);

    lua_pushstring(state, "minwidth");
    lua_rawget(state, -2);
    config->window.minwidth = luaL_optinteger(state, -1, 1);
    lua_pop(state, 1);

    lua_pushstring(state, "resizable");
    lua_rawget(state, -2);
    config->window.resizable = l_tools_optBoolean(state, -1, 1);
    lua_pop(state, 1);

    lua_pushstring(state, "bordless");
    lua_rawget(state, -2);
    config->window.bordless = l_tools_optBoolean(state, -1, 1);
    lua_pop(state, 1);

    lua_pushstring(state, "window");
    lua_rawget(state, -2);
    config->window.window = l_tools_optBoolean(state, -1, 1);
    lua_pop(state, 1);

    lua_pushstring(state, "stats");
    lua_rawget(state, -2);
    config->window.stats = l_tools_optBoolean(state, -1, 0);
    lua_pop(state, 1);

}

static char const bootScript[] =
"package.path = '?.lua;?/init.lua'\n"
"love.update = function(dt) if love.keyboard.isDown('esc') then love.event.quit() end end\n"
"love.draw = function() end \n"
"love.load = function() end \n"
"love.mousepressed = function(k,x,y) end\n"
"love.mousereleased = function(k,x,y) end\n"
"love.keypressed = function(k) end\n"
"love.keyreleased = function(k) end\n"
"love.joystickpressed = function(id, button) end\n"
"love.joystickreleased = function(id, button) end\n"
"love.wheelmoved= function(y) end\n"
"love.quit = function() end\n"
"love.focus = function(f) end\n"
"love.textinput = function(t) end\n"
"local conf = {\n"
"  window = {\n"
"    width = 800,\n"
"    height = 600\n"
"  }\n"
"}\n"
"local confFunc = loadfile(\"conf.lua\")\n"
"if confFunc then\n"
"  confFunc()\n"
"  love.conf(conf)\n"
"end\n"
"return conf\n"
;

int l_boot(lua_State* state, love_Config *config) {
    if(luaL_dostring(state, bootScript)) {
        return 1;
    }

    setConfDefault(state, config);

    return 0;
}

static char const no_game_Script[] =
"package.path = '?.lua;?/init.lua'\n"
"love.update = function(dt) if love.keyboard.isDown('esc') then love.event.quit() end end\n"
"love.draw = function() love.graphics.setBackgroundColor(189, 86, 78) love.graphics.print('No main.lua', love.window.getWidth() / 2 - 100, love.window.getHeight()/2) end \n"
"love.load = function() love.graphics.setFont(24) love.window.setTitle('No game') end \n"
"love.mousepressed = function(k,x,y) end\n"
"love.mousereleased = function(k,x,y) end\n"
"love.keypressed = function(k) end\n"
"love.keyreleased = function(k) end\n"
"love.wheelmoved= function(y) end\n"
"love.joystickpressed = function(id, button) end\n"
"love.joystickreleased = function(id, button) end\n"
"love.quit = function() end\n"
"love.focus = function() end\n"
"love.textinput = function(t) end\n"
"local conf = {\n"
"  window = {\n"
"    width = 800,\n"
"    height = 600\n"
"  }\n"
"}\n"
"local confFunc = loadfile(\"conf.lua\")\n"
"if confFunc then\n"
"  confFunc()\n"
"  love.conf(conf)\n"
"end\n"
"return conf\n"
;


int l_no_game(lua_State* state, love_Config *config) {
    if(luaL_dostring(state, no_game_Script)) {
        return 1;
    }

    setConfDefault(state, config);

    return 0;
}
