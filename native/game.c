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

#include "../src/3rdparty/chipmunk/include/chipmunk/chipmunk.h"

cpVect pos;
cpVect gpos;

cpBody *ballBody;
cpSpace *space;
cpShape *ballShape;

cpFloat radius = 25;
cpFloat mass = 6;

cpBody* staticBody;

void game_load() {
    graphics_setTitle("Clove in native");

    cpVect gravity = cpv(0, 100);

    // Create an empty space
    space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);

    /*
    staticBody = cpSpaceGetStaticBody(space);
    cpShape* shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0,64), cpv(164,64), 0.0f));
    cpShapeSetElasticity(shape, 1.0f);
    cpShapeSetFriction(shape, 1.0f);
    cpBodySetPosition(staticBody, cpv(100,200));
    */

    staticBody = cpSpaceAddBody(space, cpBodyNewStatic());//cpBodyNew(mass, cpMomentForBox(mass,size,size)));
    cpBodySetPosition(staticBody, cpv(100,200));

    cpShape *shape = cpSpaceAddShape(space, cpBoxShapeNew(staticBody, 100, 32, 0.0));
    cpShapeSetElasticity(shape, 0.0f);
    cpShapeSetFriction(shape, 0.7f);

    cpFloat moment = cpMomentForCircle(mass, 0, 25, cpvzero);

    ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPosition(ballBody, cpv(150, 25));

    ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, 25, cpv(0,-15)));
    cpShapeSetFriction(ballShape, 0.7);
}

void game_update(float delta) {
    // timer += delta * 2.0f;
    pos = cpBodyGetPosition(ballBody);
    gpos = cpBodyGetPosition(staticBody);

    cpSpaceStep(space, delta);
}

void game_draw() {
    graphics_setBackgroundColor(.8f, .6f, .5f, 1);

    graphics_setColor(.4f,.43f,.7f,1.0f);
    graphics_geometry_rectangle(true, gpos.x, gpos.y, 132, 32, 0, 1, 1, 0, 0);

    graphics_geometry_drawCircle(pos.x, pos.y, 25, 12);

}

void game_quit() {
    cpShapeFree(ballShape);
    cpBodyFree(ballBody);
    cpSpaceFree(space);
}

