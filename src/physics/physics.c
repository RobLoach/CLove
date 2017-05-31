/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "physics.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static struct {} moduleData;

void physics_newWord(physics_PhysicsData* physics, cpFloat grav_x, cpFloat grav_y)
{
    physics->space = cpSpaceNew();
    physics->gravity = cpv(grav_x, grav_y);
    cpSpaceSetGravity(physics->space, physics->gravity);
}

void physics_update(physics_PhysicsData* physics, float dt)
{
    cpSpaceStep(physics->space, dt);
}

/*********** Body stuff goes here ****************/
void physics_newCircleBody(physics_PhysicsData* physics, cpFloat mass, cpFloat radius, cpFloat moment, cpVect offset, char *type)
{


    cpFloat _moment = 0;
    // If we don't provide a default moment then we let chipmunk calculate one
    if (moment == 0)
        cpMomentForCircle(mass, 0, radius, offset);

    cpBody* body;
    if (strcmp(type, "static"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNewStatic());
    } else if (strcmp(type, "dynamic"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNew(mass, moment == 0 ? _moment : moment));
    }

}

cpVect physics_getBodyPosition(cpBody* body)
{
    cpVect pos = cpBodyGetPosition(body);
    return pos;
}

void physics_setBodyPosition(cpBody* body, cpFloat x, cpFloat y)
{
    cpBodySetPosition(body, cpv(x, y));
}

/*********** End of Body stuff ****************/


/*********** Shape stuff goes here ****************/
void physics_newCircleShape(physics_PhysicsData* physics, cpBody* body, cpFloat radius, cpVect offset)
{
    cpShape* shape = cpSpaceAddShape(physics->space, cpCircleShapeNew(body, radius, offset));
}

void physics_setShapeFriction(cpShape* shape, float v)
{
    cpShapeSetFriction(shape, v);
}

/*********** End of Shape stuff ****************/
