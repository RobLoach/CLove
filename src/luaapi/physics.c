/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "physics.h"

static struct
{
    int physicsMT;
} moduleData;

static int l_physics_gc(lua_State* state)
{

}

static luaL_Reg const physicsMetatableFuncs[] =
{
    {NULL, NULL}
};


static lua_Reg const physicsFreeFuncs[] =
{
    {NULL, NULL}
};

void l_physics_register(lua_State *state)
{
    l_tools_registerFuncsInModule(state, "physics", physicsFreeFuncs);
    moduleData.physicsMT = l_tools_makeTypeMetatable(state, physicsMetatableFuncs);
}

