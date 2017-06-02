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
    int bodyMT;
    int shapeMT;

} moduleData;

static int l_physics_newSpace(lua_State* state)
{

    float x = l_tools_toNumberOrErrorPlusMsg(state, 1, "You must provide an x gravity value!");
    float y = l_tools_toNumberOrErrorPlusMsg(state, 1, "You must provide an y gravity value!");

    l_physics_PhysicsData* physics = lua_newuserdata(state, sizeof(l_physics_PhysicsData));
    physics_newSpace(&physics->physics, x, y);

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.physicsMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_setSpaceIterations(lua_State* state)
{

    physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);

    int iterations = l_tools_toIntegerOrError(state, 2);

    physics_setSpaceIterations(&physics->physics, iterations);

    return 0;
}

static int l_physics_setSpaceSleepTime(lua_State* state)
{

    physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);

    float sleep = l_tools_toNumberOrError(state, 2);

    physics_setSpaceSleepTime(&physics->physics, sleep);

    return 0;
}


static int l_physics_newBoxBody(lua_State* state)
{

    l_physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);
    float mass = l_tools_toNumberOrError(state, 2);
    float width = l_tools_toNumberOrError(state, 3);
    float height = luaL_optnumber(state, 3, width);
    float moment = luaL_optnumber(state, 4, 0);
    const char* type = luaL_optstring(state, 5, "dynamic");

    physics_newBoxBody(&physics->physics, mass, width, height, moment, type);

    //TODO see if 'moduleData.bodyMT' needs to be different for different types of body!
    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.bodyMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_newCircleBody(lua_State* state)
{
    l_physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);
    float mass = l_tools_toNumberOrError(state, 2);
    float radius = l_tools_toNumberOrError(state, 3);
    float moment = l_tools_toNumberOrError(state, 4);
    cpVect offset = cpvzero;
    offset.x = l_tools_toNumberOrError(state, 5);
    offset.y = l_tools_toNumberOrError(state, 6);
    const char* type = luaL_optstring(state, 7, "dynamic");

    physics_newCircleBody(&physics->physics, mass, radius, moment, offset, type);

    //TODO see if 'moduleData.bodyMT' needs to be different for different types of body!
    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.bodyMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getBodyTorque(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    lua_pushnumber(state, physics_getBodyTorque(&body->body));

    return 1;
}

static int l_physics_getBodyCenterOfGravity(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    cpVect vec = physics_getBodyCenterOfGravity(&body->body);
    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_getBodyAngularVelocity(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    lua_pushnumber(state, physics_getBodyAngularVelocity(&body->body));

    return 1;
}

static int l_physics_getBodyForce(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    cpVect vec = physics_getBodyForce(&body->body);

    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_getBodyMass(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    lua_pushnumber(state, physics_getBodyMass(&body->body));

    return 1;
}

static int l_physics_getBodyAngle(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    lua_pushnumber(state, physics_getBodyAngle(&body->body));

    return 1;
}

static int l_physics_getBodyPosition(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    cpVect vec = physics_getBodyPosition(body);

    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_setBodyAngle(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyAngle(&body->body, value);

    return 0;
}

static int l_physics_setBodyAngularVelocity(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyAngularVelocity(&body->body, value);

    return 0;
}

static int l_physics_setBodyCenterOfGravity(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyAngularVelocity(&body->body, value);

    return 0;
}

static int l_physics_setBodyForce(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    physics_setBodyForce(&body->body, vec);

    return 0;
}

static int l_physics_setBodyMass(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyMass(&body->body, value);

    return 0;
}

static int l_physics_setBodyTorque(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyTorque(&body->body, value);

    return 0;
}

static int l_physics_setBodyVelocity(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    physics_setBodyVelocity(&body->body, vec);

    return 0;
}

static int l_physics_setBodyPosition(lua_State* state)
{
    l_physics_Body* body = l_physics_toPhysicsBody(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    physics_setBodyPosition(&body->body, vec);

    return 0;
}

static int l_physics_newCircleShape(lua_State* state)
{
    l_physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);
    l_physics_Body* body = l_physics_toPhysicsBody(state, 2);
    float radius = l_tools_toNumberOrError(state, 3);

    cpVect offset = cpvzero;
    offset.x = l_tools_toNumberOrError(state, 4);
    offset.y = l_tools_toNumberOrError(state, 5);

    physics_newCircleShape(physics, &body->body, radius, offset);

    //TODO see if 'moduleData.bodyMT' needs to be different for different types of body!
    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_newBoxShape(lua_State* state)
{
    l_physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);
    l_physics_Body* body = l_physics_toPhysicsBody(state, 2);
    float width = l_tools_toNumberOrError(state, 3);
    float height = l_tools_toNumberOrError(state, 4);
    float radius = l_tools_toNumberOrError(state, 5);

    physics_newBoxShape(physics, &body->body, width, height, radius);

    //TODO see if 'moduleData.bodyMT' needs to be different for different types of body!
    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_newShape(lua_State* state)
{
    l_physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);
    l_physics_Body* body = l_physics_toPhysicsBody(state, 2);
    float x1 = l_tools_toNumberOrError(state, 3);
    float y1 = l_tools_toNumberOrError(state, 4);
    float x2 = l_tools_toNumberOrError(state, 5);
    float y2 = l_tools_toNumberOrError(state, 6);
    float radius = l_tools_toNumberOrError(state, 7);

    physics_newShape(physics, &body->body, x1, y1, x2, y2, radius);

    //TODO see if 'moduleData.bodyMT' needs to be different for different types of body!
    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getShapeDensity(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    lua_pushnumber(state, physics_getShapeDensity(&shape->shape));

    return 1;
}


static int l_physics_getElasticity(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    lua_pushnumber(state, physics_getElasticity(&shape->shape));

    return 1;
}

static int l_physics_getFriction(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    lua_pushnumber(state, physics_getFriction(&shape->shape));

    return 1;
}

static int l_physics_getMass(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    lua_pushnumber(state, physics_getMass(&shape->shape));

    return 1;
}

static int l_physics_getMoment(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    lua_pushnumber(state, physics_getMoment(&shape->shape));

    return 1;
}

static int l_physics_setShapeFriction(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeFriction(&shape->shape, value);

    return 0;
}

static int l_physics_setShapeDensity(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeDensity(&shape->shape, value);

    return 0;
}

static int l_physics_setShapeElasticity(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeElasticity(&shape->shape, value);

    return 0;
}

static int l_physics_setShapeMass(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeMass(&shape->shape, value);

    return 0;
}

static int l_physics_setShapeBody(lua_State* state)
{
    l_physics_Shape* shape = l_physics_toPhysicsShape(state, 1);
    l_physics_Body* body = l_physics_toPhysicsBody(state, 2);

    physics_setShapeBody(&shape->shape, &body->body);

    return 0;
}


static int l_physics_gc(lua_State* state)
{
    l_physics_PhysicsData* physics = l_physics_toPhysicsData(state, 1);
    physics_free(&physics->physics);

    luaL_unref(state, LUA_REGISTRYINDEX, physics->physicsRef);

    return 0;
}

static luaL_Reg const physicsSpaceMetatableFuncs[] =
{
    {"__gc",   l_physics_gc},
    {NULL, NULL}
};

static luaL_Reg const physicsBodyMetatableFuncs[] =
{
    {"getTorque",                    l_physics_getBodyTorque},
    {"getAngle",                     l_physics_getBodyAngle},
    {"getAngularVelocity",           l_physics_getBodyAngularVelocity},
    {"getCenterOfGravity",           l_physics_getBodyCenterOfGravity},
    {"getForce",                     l_physics_getBodyForce},
    {"getMass",                      l_physics_getBodyMass},
    {"getPosition",                  l_physics_getBodyPosition},
    {"getBody",                      l_physics_getBody},

    {"setAngle",                     l_physics_setBodyAngle},
    {"setAngularVelocity",           l_physics_setBodyAngularVelocity},
    {"setCenterOfGravity",           l_physics_setBodyCenterOfGravity},
    {"setForce",                     l_physics_setBodyForce},
    {"setMass",                      l_physics_setBodyMass},
    {"setPosition",                  l_physics_setBodyPosition},
    {"setToruq",                     l_physics_setBodyTorque},
    {"setVelocity",                  l_physics_setBodyVelocity},

    {NULL, NULL}
};

//TODO add for shape

static luaL_Reg const physicsFreeFuncs[] =
{
    {"setSpaceIterations",    l_physics_setSpaceIterations},
    {"setSpaceSleep",         l_physics_setSpaceSleepTime},
    {"newSpace",              l_physics_newSpace},
    {"newBoxBody",            l_physics_newBoxBody},
    {"newCircleBody",         l_physics_newCircleBody},
    {"newBoxShape",           l_physics_newBoxShape},
    {"newCircleShape",        l_physics_newCircleShape},
    {"newShape",              l_physics_newShape},

    {NULL, NULL}
};

int l_physics_register(lua_State *state)
{
    l_tools_registerFuncsInModule(state, "physics", physicsFreeFuncs);
    moduleData.physicsMT = l_tools_makeTypeMetatable(state, physicsSpaceMetatableFuncs);
    return 1;
}

l_checkTypeFn(l_physics_isPhysicsData, moduleData.physicsMT);
l_checkTypeFn(l_physics_isPhysicsBody, moduleData.bodyMT);
l_checkTypeFn(l_physics_isPhysicsShape, moduleData.shapeMT);

l_toTypeFn(l_physics_toPhysicsBody, l_physics_Body);
l_toTypeFn(l_physics_toPhysicsShape, l_physics_Shape);
l_toTypeFn(l_physics_toPhysicsData, l_physics_PhysicsData);
