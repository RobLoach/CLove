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
#include "../src/luaapi/event.h"

// 3d
#include "../src/graphics/3d/obj_loader.h"

#include "../src/3rdparty/chipmunk/include/chipmunk/chipmunk.h"

cpVect pos;
cpVect gpos;

cpBody *ballBody;
cpSpace *space;
cpShape *ballShape;
cpShape *shape;

cpFloat radius = 25;
cpFloat mass = 6;

cpBody* staticBody;

static cpBool OnImpact(cpArbiter *arb, cpSpace *space, void *data)
{
    printf("%s \n", "message");
    return cpTrue;
}


#define a_type 1
#define b_type 2
cpArbiter *arb;

static void
postStepRemove(cpSpace *space, cpShape *shape, void *unused)
{
  cpSpaceRemoveShape(space, shape);
  cpSpaceRemoveBody(space, ballBody);

  cpShapeFree(shape);
  cpBodyFree(ballBody);
}

static cpBool
begin(cpArbiter* arb, cpSpace* space, void *data)
{
    //CP_ARBITER_GET_SHAPES(arb, shape, ballShape);

    printf("%s \n", "working");

    return cpTrue;
}

void game_load() {
/*
    graphics_setTitle("Clove in native");

    cpVect gravity = cpv(0, 100);

    // Create an empty space
    space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);

    //
    staticBody = cpSpaceGetStaticBody(space);
    cpShape* shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0,64), cpv(164,64), 0.0f));
    cpShapeSetElasticity(shape, 1.0f);
    cpShapeSetFriction(shape, 1.0f);
    cpBodySetPosition(staticBody, cpv(100,200));
    //

    staticBody = cpSpaceAddBody(space, cpBodyNewStatic());//cpBodyNew(mass, cpMomentForBox(mass,size,size)));
    cpBodySetPosition(staticBody, cpv(100,200));
    shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(132, -12), cpv(0, -12), 0.0f));//cpSpaceAddShape(space, cpBoxShapeNew(staticBody, 120, 32, 0.0));
    cpShapeSetElasticity(shape, 0.0f);
    cpShapeSetFriction(shape, 0.7f);

    cpFloat moment = cpMomentForCircle(mass, 0, 25, cpvzero);

    ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPosition(ballBody, cpv(150, 25));


    ballShape = cpSpaceAddShape(space, cpBoxShapeNew(ballBody, 16, 12, 0.0f));
    cpShapeSetFriction(ballShape, 0.7);

    cpShapeSetCollisionType(ballShape, a_type);
    cpShapeSetCollisionType(shape, b_type);

    cpCollisionHandler *handler = cpSpaceAddCollisionHandler(space, a_type, b_type);
    handler->beginFunc = begin;

*/
}

cpFloat angle;

void game_update(float delta) {
    /*
    // timer += delta * 2.0f;
    pos = cpBodyGetPosition(ballBody);
    gpos = cpBodyGetPosition(staticBody);

    angle = cpBodyGetAngle(ballBody);

    if (keyboard_ispressed(SDLK_ESCAPE))
        l_running = false;

    if (keyboard_ispressed(SDLK_d))
    {
        pos.x = pos.x + 50 * delta;
        cpBodySetPosition(ballBody, pos);
    }
    if (keyboard_ispressed(SDLK_a))
    {
        pos.x = pos.x - 50 * delta;
        cpBodySetPosition(ballBody, pos);
    }


    cpSpaceStep(space, delta);
*/
}

void game_draw() {
/*
    graphics_setBackgroundColor(.8f, .6f, .5f, 1);

    graphics_setColor(.4f,.43f,.7f,1.0f);
    graphics_geometry_rectangle(true, gpos.x, gpos.y, 132, 32, 0, 1, 1, 0, 0);

    graphics_geometry_rectangle(true, pos.x, pos.y, 16, 16, angle, 1, 1, 0, 0);
    //graphics_geometry_lineCircle(pos.x, pos.y, 25, 12, angle, 1, 1, 0, 0);
*/
}

void game_quit() {
 //   cpShapeFree(ballShape);
 //   cpBodyFree(ballBody);
 //   cpSpaceFree(space);
}

