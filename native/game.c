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
#include "../src/keyboard.h"

// 3d
#include "../src/graphics/3d/obj_loader.h"

static float timer = 0.0f;

obj_loader* obj;
graphics_Image* image;

void game_load() {
    /*
    printf("Clove: Hello from native game! \n");
    graphics_setTitle("Clove in native");

    graphics_set_camera_3d(65, 800.0f/600.0f,1.0f,3000);
    graphics_set_look_at(1.2,1.2,-140.2, 0,-20,20, 0, -1, 0);

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

    */
}

float x = 0;
float y = 0;
float z = 0;

void game_update(float delta) {
   // timer += delta * 2.0f;
}

static const graphics_Quad defaultQuad = {
    .x = 0.0f,
    .y = 0.0f,
    .w = 1.0f,
    .h = 1.0f
};

void game_draw() {
    /*
    graphics_setBackgroundColor(.8f, .6f, .5f, 1);
    graphics_setColor(.4f,.43f,.7f,1.0f);

    matrixstack_push();

    matrixstack_translate(0,-300,-1000);

    graphics_Image_draw(image, &defaultQuad, 30, 30, 0, 1, 1, 0, 0, 0, 0);

    graphics_ObjLoader_draw(obj, image, 30, 30, -4000);

    matrixstack_pop();
    */
}

void game_quit() {
    /*
    // free objects here
    graphics_Image_free(image);
    */
}

