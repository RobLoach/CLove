/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "3rdparty/SDL2/include/SDL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/utils.h"
#include "luaapi/mouse.h"

extern SDL_Window* graphics_getWindow(void);

static struct {
    int x, y;
    int dx, dy;
    int visible;
    int wheel;
    int button;
} moduleData;

void mouse_setButton(int button) {
    moduleData.button = button;
}

static int buttonEnum(const char *str) {
    int res = 0;
    if (strncmp (str,"l",1) == 0)
        res = SDL_BUTTON_LEFT;
    if (strncmp (str,"r",1) == 0)
        res = SDL_BUTTON_RIGHT;
    if (strncmp (str,"m",1) == 0)
        res = SDL_BUTTON_MIDDLE;
    if (strncmp (str,"x1",2) == 0)
        res = SDL_BUTTON_X1;
    if (strncmp (str,"x2",2) == 0)
        res = SDL_BUTTON_X2;
    return res;
}

void mouse_mousewheel(int y) {
    moduleData.wheel = y;
    l_mouse_wheelmoved(moduleData.wheel);
}

int mouse_getwheel() {
    return moduleData.wheel;
}

void mouse_mousemoved(int x, int y) {
    if(moduleData.x == x && moduleData.y == y) {
        return;
    }

    moduleData.dx = x - moduleData.x;
    moduleData.dy = y - moduleData.y;
    moduleData.x = x;
    moduleData.y = y;
}

void mouse_mousepressed(int x, int y, int button) {
    if (button == SDL_BUTTON_WHEEL_UP || button == SDL_BUTTON_WHEEL_DOWN) {
        l_mouse_pressed(moduleData.x, moduleData.y, button);
        mouse_mousemoved(moduleData.x, moduleData.y);
    }else{
        l_mouse_pressed(x, y, button);
        mouse_mousemoved(x, y);
    }
}

void mouse_mousereleased(int x, int y, int button) {
    mouse_mousemoved(x, y);
    l_mouse_released(x, y, button);
}

void mouse_getPosition(int *x, int *y) {
    *x = moduleData.x;
    *y = moduleData.y;
}

int mouse_isDown(const char *str) {
    int x = buttonEnum(str);
    if (moduleData.button == x)
        return moduleData.button;

    return 0;
}

int mouse_isVisible(void) {
    return moduleData.visible;
}

int mouse_getX(void) {
    return moduleData.x;
}

int mouse_getY(void) {
    return moduleData.y;
}

void mouse_setPosition(int x, int y) {
        SDL_WarpMouseInWindow(graphics_getWindow(), x, y);
}

void mouse_setVisible(int b) {
    moduleData.visible = !!b;
    SDL_ShowCursor(b ? SDL_ENABLE : SDL_DISABLE);
}

void mouse_setX(int x) {
        SDL_WarpMouseInWindow(graphics_getWindow(), x, moduleData.y);
}

void mouse_setY(int y) {
        SDL_WarpMouseInWindow(graphics_getWindow(), moduleData.x, y);
}
