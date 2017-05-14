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

#include "../src/graphics/graphics.h"
#include "../src/graphics/geometry.h"
#include "../src/graphics/matrixstack.h"

// 3d
#include "../src/graphics/3d/obj_loader.h"

static float timer = 0.0f;

obj_loader* obj;
graphics_Image* image;

void game_load() {
    printf("Clove: Hello from native game! \n");
    graphics_setTitle("Clove in native");
    //graphics_set_camera_3d(60, 800.0f/600.0f,0.1f,1000);

    //graphics_set_look_at(1.2,1.2,-140.2, 0,-20,20, 0, -1, 0);

    //graphics_set_camera_2d(0,800,600,0,0.1f,100);

    image_ImageData* data = malloc(sizeof(image_ImageData));
    image_ImageData_new_with_filename(data, "hey.png");

    image = malloc(sizeof(graphics_Image));
    graphics_Image_new_with_ImageData(image, data);

    obj = malloc(sizeof(obj_loader));
    int res = graphics_ObjLoader_new(obj, "cube.obj");

    printf("%d \n", res);

    //image_ImageData_free(data);
    //free(data);
}

void game_update(float delta) {
    timer += delta * 2.0f;
}

void game_draw() {

    matrixstack_push();
    matrixstack_translate(100, 150, -0);

    graphics_setBackgroundColor(.8f, .6f, .5f, 1);
    graphics_setColor(.4f,.43f,.7f,1.0f);


    graphics_geometry_fillCircle(120, 0, 32, 12);
    graphics_geometry_fillCircle(230, 0, 16, 12);
    graphics_geometry_rectangle(true, 420, -10, 132, 32, 0, 1, 1, 0, 0);


    graphics_ObjLoader_draw(obj, image, 120, 100, 3);
    graphics_ObjLoader_draw(obj, image, 230, -190, -2);


    matrixstack_pop();
}

void game_quit() {
    // free objects here
    graphics_Image_free(image);
}

