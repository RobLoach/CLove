/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../3rdparty/lua/lua.h"
#include "../physics/physics.h"

#include "tools.h"

typedef struct
{
    cpBody* body;
    int bodyRef;
}   l_physics_Body;

typedef struct
{
    cpShape* shape;
    int shapeRef;
}   l_physics_Shape;

typedef struct
{
    physics_PhysicsData* physics;
    int physicsRef;
} l_physics_PhysicsData;

int l_physics_register(lua_State* state);
