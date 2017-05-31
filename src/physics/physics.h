/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../3rdparty/chipmunk/include/chipmunk/chipmunk.h"

typedef struct
{
    cpSpace* space;

    cpVect gravity;

} physics_PhysicsData;

// Global/random stuff goes here
void physics_newWord(physics_PhysicsData* physics, cpFloat grav_x, cpFloat grav_y);
void physics_update(physics_PhysicsData* physics, float dt);

// Body stuff goes here
void physics_newCircleBody(physics_PhysicsData* physics, cpFloat mass, cpFloat radius, cpFloat moment, cpVect offset, char* type);
void physics_setBodyPosition(cpBody* body, cpFloat x, cpFloat y);

//Shape stuff goes here
void physics_newCircleShape(physics_PhysicsData* physics, cpBody* body, cpFloat radius, cpVect offset);
