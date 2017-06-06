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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../3rdparty/lua/lua.h"
#include "../3rdparty/lua/lauxlib.h"

#include "../3rdparty/chipmunk/include/chipmunk/chipmunk_structs.h"
#include "../3rdparty/chipmunk/include/chipmunk/chipmunk.h"

#include "../timer/timer.h"

#include "tools.h"

typedef struct
{
    cpSpace* space;
    cpVect gravity;
} physics_PhysicsData;

typedef struct
{
    physics_PhysicsData* physics;
    cpBody* body;
}   l_physics_Body;

typedef struct
{
    physics_PhysicsData* physics;
    cpShape* shape;
}   l_physics_Shape;

typedef struct
{
    physics_PhysicsData* physics;
} l_physics_PhysicsData;

int l_physics_newSpace(lua_State* state);
int l_physics_newBoxBody(lua_State* state);
int l_physics_newCircleBody(lua_State* state);
int l_physics_newCircleShape(lua_State* state);
int l_physics_newBoxShape(lua_State* state);
int l_physics_newShape(lua_State* state);

int l_physics_register(lua_State* state);

bool l_physics_isPhysicsShape(lua_State* state, int index);
l_physics_Shape* l_physics_toPhysicsShape(lua_State* state, int index);

bool l_physics_isPhysicsBody(lua_State* state, int index);
l_physics_Body* l_physics_toPhysicsBody(lua_State* state, int index);

bool l_physics_isPhysicsData(lua_State* state, int index);
l_physics_PhysicsData* l_physics_toPhysicsData(lua_State* state, int index);
