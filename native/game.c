/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "game.h"

#include <stdbool.h>
#include <stdio.h>

//#include "../src/graphics/graphics.h"
//#include "../src/graphics/geometry.h"

static float timer = 0.0f;

void game_load() {
    /*printf("Clove: Hello from native game! \n");
    graphics_setTitle("Clove in native");
    */
}

void game_update(float delta) {
    /*timer += delta * 2.0f;

    //silly hack to not spam the console
    if (timer / 2.0f > 0.0f)
        printf("%s %f \n", "Timer's value is: ", timer);
        */
}

void game_draw() {
    /*
    graphics_setBackgroundColor(.8f, .6f, .5f, 1);
    graphics_setColor(.4f,.43f,.7f,1.0f);
    graphics_geometry_drawCircle(100, 100, 32, 12);
    graphics_geometry_fillCircle(100, 150, 16, 12);
    graphics_geometry_rectangle(true, 200, 200, 32, 32, 0, 1, 1, 0, 0);
    */
}

void game_quit() {
    // free objects here
}

