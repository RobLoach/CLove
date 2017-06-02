/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include <stdbool.h>

#include "../3rdparty/lua/lua.h"
#include "../physics/physics.h"

#include "tools.h"

typedef struct
{
    cpBody body;
    int bodyRef;
}   l_physics_Body;

typedef struct
{
    cpShape shape;
    int shapeRef;
}   l_physics_Shape;

typedef struct
{
    physics_PhysicsData physics;
    int physicsRef;
} l_physics_PhysicsData;

int l_physics_register(lua_State* state);

bool l_physics_isPhysicsShape(lua_State* state, int index);
l_physics_Shape* l_physics_toPhysicsShape(lua_State* state, int index);

bool l_physics_isPhysicsBody(lua_State* state, int index);
l_physics_Body* l_physics_toPhysicsBody(lua_State* state, int index);

bool l_physics_isPhysicsData(lua_State* state, int index);
l_physics_PhysicsData* l_physics_toPhysicsData(lua_State* state, int index);
