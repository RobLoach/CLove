/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "mainactivity.h"

#ifdef CLOVE_WEB
#include <emscripten.h>
#endif

typedef struct {
    lua_State *luaState;
    int errhand;
} MainLoopData;

static void quit_function(lua_State* state) {
    lua_getglobal(state, "love");
    lua_pushstring(state, "quit");
    lua_rawget(state, -2);
    lua_pop(state, 1);
}

static void love_focus(lua_State* state) {
    lua_getglobal(state, "love");
    lua_pushstring(state, "focus");
    lua_rawget(state, -2);
    lua_pushboolean(state, graphics_hasFocus());
    lua_call(state, 1, 0);
}

static void main_clean(lua_State* state) {
    joystick_close();
    graphics_destroyWindow();
	filesystem_free();
	/* There is a nasty bug on Windows that
       causes lua_close to give a segment fault. */
    lua_close(state);
    audio_close();
}

//NOTE: Not yet fully implemented. I'm still playing around with it !!
/* Get main.lua from *.clove.tar or trow error */
static void main_load(lua_State* lua, char* argv[], love_Config* config) {
    //argv[1] = package name

    // in case no *.clove.tar
    if (argv[1] == NULL) {
        int err = luaL_dofile(lua,"main.lua");

        if (err == 1){
            l_no_game(lua, config);
            printf("%s \n", lua_tostring(lua, -1));
        } else if (err == 0)
            luaL_dofile(lua,"main.lua");

    } // *.clove.tar is required
    else {
        mtar_t tar;
        mtar_header_t header;

        mtar_open(&tar, argv[1], "r");

        while (  (mtar_read_header(&tar, &header)) != MTAR_ENULLRECORD ) {
            //printf("%s \n", header.name);
            mtar_next(&tar);
        }

        /* Used to control love.filesystem.require.
         * See examples folder -> run package */
#ifndef CLOVE_TAR
#define CLOVE_TAR 1
#endif

        char* buffer;
        int numberOfScripts = atoi(argv[2]);
        for (int i = 0; i < numberOfScripts; i++) {
            mtar_find(&tar, argv[i + 3], &header);

            buffer = calloc(1, header.size+1);
            mtar_read_data(&tar, buffer, header.size);
            //printf("%s \n", buffer);

            luaL_dostring(lua, buffer );
            lua_pcall(lua, 0, 0, 0);
        }

        luaL_dofile(lua, "main.lua");

        SAFE_FREE(buffer);

    }
}

void main_loop(void *data) {
    MainLoopData* loopData = (MainLoopData*)data;

    timer_step();
    love_focus(loopData->luaState);
    matrixstack_origin();
    lua_rawgeti(loopData->luaState, LUA_REGISTRYINDEX, loopData->errhand);
    lua_getglobal(loopData->luaState, "love");
    lua_pushstring(loopData->luaState, "update");
    lua_rawget(loopData->luaState, -2);
    lua_pushnumber(loopData->luaState, timer_getDelta());

    pcall(loopData->luaState, 1);

#ifdef USE_NATIVE
    game_update((float)timer_getDelta());
#endif

    if (swap_At == 1){
        if(luaL_dofile(loopData->luaState, "main.lua")) {
            printf("Error: %s\n", lua_tostring(loopData->luaState, -1));
        }
    }

    graphics_clear();

    lua_pushstring(loopData->luaState, "draw");
    lua_rawget(loopData->luaState, -2);

    pcall(loopData->luaState, 0);

#ifdef USE_NATIVE
    game_draw();
#endif

    graphics_swap();

    lua_pop(loopData->luaState, 1);

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
        switch(event.wheel.type) {
        case SDL_MOUSEWHEEL:
            mouse_mousewheel(event.wheel.y);
            int _what = event.wheel.y == 1 ? SDL_BUTTON_WHEEL_UP : SDL_BUTTON_WHEEL_DOWN;
            mouse_mousepressed(event.button.x, event.button.y,
                               _what);
            mouse_setButton(event.button.button);
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
            mouse_mousepressed(event.button.x, event.button.y, event.button.button);
            mouse_setButton(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            mouse_mousereleased(event.button.x, event.button.y,
                                event.button.button);
            mouse_setButton(0);
            break;
        case SDL_JOYDEVICEADDED:
            joystick_added(event.jdevice.which);
            break;
        case SDL_JOYDEVICEREMOVED:
            joystick_remove(event.jdevice.which);
            break;
        case SDL_JOYAXISMOTION:
            break;
        case SDL_JOYBUTTONDOWN:
            joystick_buttonDown(event.jbutton.which, event.jbutton.button, event.jbutton.state);
            break;
        case SDL_JOYBUTTONUP:
            joystick_buttonUp(event.jbutton.which, event.jbutton.button, event.jbutton.state);
            break;
#ifdef CLOVE_DESKTOP
        case SDL_QUIT:
            quit_function(loopData->luaState);
            l_running = 0;
            break;
#endif
        }
    }
    audio_updateStreams();
}


void main_activity_load(int argc, char* argv[]) {
    keyboard_init();
    joystick_init();
    timer_init();

    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);

    love_Config config;

    filesystem_init(argv[0], config.window.stats);

	l_love_register(lua);
    l_audio_register(lua);
    l_event_register(lua);
    l_graphics_register(lua);
    l_image_register(lua);
    l_joystick_register(lua);
    l_keyboard_register(lua);
    l_mouse_register(lua);
    l_filesystem_register(lua);
    l_timer_register(lua);
    l_math_register(lua);
    l_system_register(lua);
    l_net_register(lua);
	l_physics_register(lua);
	l_thread_register(lua);

    l_boot(lua, &config);

    audio_init(config.window.stats);
	filesystem_setIdentity(config.window.identity);

	if (config.window.stats)
        printf("%s %s \n", "Debug: Platform:", filesystem_getOS());

    graphics_init(config.window.width, config.window.height, config.window.resizable, config.window.stats, config.window.window);
    /*
     * When we do not have a visible window we can't put
     * these propieties
     */
    if (config.window.window)
    {
        graphics_setTitle(config.window.title);
        graphics_setBordless(config.window.bordless);
        graphics_setMinSize(config.window.minwidth, config.window.minheight);
        graphics_setVsync(config.window.vsync);
        graphics_setPosition(config.window.x, config.window.y);
    }
    l_running = 1;

    main_load(lua, argv, &config);

    love_Version const * version = love_getVersion();
    if (config.window.stats > 0)
        printf("%s %s %d.%d.%d \n", "CLove version - ",
               version->codename,version->major,version->minor,version->revision);

    lua_pushcfunction(lua, errorhandler);
    lua_getglobal(lua, "love");
    lua_pushstring(lua, "load");
    lua_rawget(lua, -2);

    pcall(lua, 0);

    lua_pop(lua, 1);

#ifdef USE_NATIVE
    game_load();
#endif

    lua_pushcfunction(lua, errorhandler);

    MainLoopData mainLoopData = {
        .luaState = lua,
        .errhand = luaL_ref(lua, LUA_REGISTRYINDEX)
    };

#ifdef CLOVE_WEB
    //TODO find a way to quit(love.event.quit) love on web?
    emscripten_set_main_loop(main_loop, 60, 1);
#else
    while (l_event_running())
        main_loop(&mainLoopData);
#endif
    quit_function(lua);
    main_clean(lua);
}
