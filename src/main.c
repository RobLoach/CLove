/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#ifdef EMSCRIPTEN
# include <emscripten.h>
#endif
#include <stdio.h>

#include "3rdparty/lua/lua.h"
#include "3rdparty/lua/lauxlib.h"
#include "3rdparty/lua/lualib.h"
#include "3rdparty/SDL2/include/SDL.h"
#include "luaapi/audio.h"
#include "luaapi/mouse.h"
#include "luaapi/event.h"
#include "luaapi/graphics.h"
#include "luaapi/graphics_font.h"
#include "luaapi/image.h"
#include "luaapi/love.h"
#include "luaapi/boot.h"
#include "luaapi/keyboard.h"
#include "luaapi/mouse.h"
#include "luaapi/filesystem.h"
#include "luaapi/timer.h"
#include "luaapi/math.h"
#include "luaapi/system.h"
#include "luaapi/physics.h"
#include "love.h"

#include "graphics/graphics.h"
#include "filesystem/filesystem.h"
#include "audio/audio.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer/timer.h"

typedef struct {
  lua_State *luaState;
  int errhand;
} MainLoopData;

void quit_function(lua_State* state)
{
  lua_getglobal(state, "love");
  lua_pushstring(state, "quit");
  lua_rawget(state, -2);
  lua_call(state, 0, 0);
}

int lua_errorhandler(lua_State *state) {
  lua_Debug debug;
  int level = 0;
  while(lua_getstack(state, level, &debug)) {
      lua_getinfo(state, "Sl", &debug);
      lua_pushstring(state, debug.short_src);
      lua_pushstring(state, ":");
      lua_pushnumber(state, debug.currentline);
      lua_pushstring(state, "\n");
      ++level;
    }

  lua_concat(state, 4*level+1);
  return 1;
}

void main_loop(lua_State* luaState) {

  timer_step();

  //lua_rawgeti(luaState, LUA_REGISTRYINDEX, loopData->errhand);
  lua_getglobal(luaState, "love");
  lua_pushstring(luaState, "update");

  lua_rawget(luaState, -2);
  lua_pushnumber(luaState, timer_getDelta());
  lua_call(luaState, 1, 0);

  if (swap_At == 1){
      if(luaL_dofile(luaState, "main.lua")) {
          printf("Error: %s\n", lua_tostring(luaState, -1));
        }
    }

  graphics_clear();

  lua_pushstring(luaState, "draw");
  lua_rawget(luaState, -2);
  lua_call(luaState, 0, 0);

  graphics_swap();

  // silly hack for love.event.quit()
#ifdef WINDOWS
  event_force_quit = glfwWindowShouldClose(graphics_getWindow());
  if(event_force_quit)
      l_running = 0;

#endif //This will affect only Windows users
  //

  lua_pop(luaState, 1);
#ifdef UNIX
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
      if (event.type == SDL_WINDOWEVENT) {
          switch (event.window.event) {
            case SDL_WINDOWEVENT_ENTER:
              graphics_setMouseFocus(1);
              break;
            case SDL_WINDOWEVENT_LEAVE:
              graphics_setMouseFocus(0);
              break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
              graphics_setFocus(0);
              break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
              graphics_setFocus(1);
              break;
            default:
              break;
            }
        }
      switch(event.wheel.type)
        {
        case SDL_MOUSEWHEEL:
          mouse_mousewheel(event.wheel.y);
          int _what = event.wheel.y == 1 ? SDL_BUTTON_WHEEL_UP : SDL_BUTTON_WHEEL_DOWN;
          mouse_mousepressed(event.button.x, event.button.y,
                             _what);
          break;
        default:
          break;
        }
      switch(event.type) {
        case SDL_KEYDOWN:
          keyboard_keypressed(event.key.keysym.sym);
          break;
        case SDL_KEYUP:
          keyboard_keyreleased(event.key.keysym.sym);
          break;
        case SDL_TEXTINPUT:
          keyboard_textInput(event.text.text);
          break;
        case SDL_MOUSEMOTION:
          mouse_mousemoved(event.motion.x, event.motion.y);
          break;
        case SDL_MOUSEBUTTONDOWN:
          mouse_mousepressed(event.button.x, event.button.y,
                             event.button.button);
          break;
        case SDL_MOUSEBUTTONUP:
          mouse_mousereleased(event.button.x, event.button.y,
                              event.button.button);
          break;
#ifndef EMSCRIPTEN
        case SDL_QUIT:
          quit_function(loopData->luaState);
          l_running = 0;
#endif
        }
    }
#endif
}

int main(int argc, char* argv[]) {
  lua_State *lua = luaL_newstate();
  luaL_openlibs(lua);

  love_Config config;

  l_love_register(lua);
  l_math_register(lua);
  l_system_register(lua);
  l_audio_register(lua);
  l_event_register(lua);
  l_image_register(lua);
  l_graphics_register(lua);
  l_keyboard_register(lua);
  l_mouse_register(lua);
  l_filesystem_register(lua);
  l_timer_register(lua);
  l_physics_register(lua);

  l_boot(lua, &config);

  graphics_init(config.window.width, config.window.height);
  keyboard_init();
  timer_init();
  audio_init();

  if(luaL_dofile(lua, "main.lua")){
      printf("Error: %s\n", lua_tostring(lua, -1));
      l_no_game(lua,&config);
    }

  love_Version const * version = love_getVersion();
  printf("%s %s %i %i %i \n", "Love code name: ",version->codename,version->major,version->minor,version->revision);

  lua_pushcfunction(lua, lua_errorhandler);
  lua_getglobal(lua, "love");
  lua_pushstring(lua, "load");
  lua_rawget(lua, -2);
  lua_call(lua, 0, 0);

  lua_pushcfunction(lua, lua_errorhandler);
  MainLoopData mainLoopData;
  mainLoopData.luaState = lua;
  mainLoopData.errhand = luaL_ref(lua, LUA_REGISTRYINDEX);

#ifdef EMSCRIPTEN
  //TODO find a way to quit(love.event.quit) love on web?
  emscripten_set_main_loop_arg(main_loop, &mainLoopData, 0, 1);
#else
  while(l_event_running())
      main_loop(lua);

  if(!l_event_running())
    quit_function(lua);
#endif
  audio_close ();
  lua_close(lua);
  return 0;
}
