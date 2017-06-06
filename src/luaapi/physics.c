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

    l_physics_Body* body;
    l_physics_Shape* shape;
    l_physics_PhysicsData* physics;

} moduleData;

int l_physics_newSpace(lua_State* state)
{

    float x = l_tools_toNumberOrError(state, 1);
    float y = l_tools_toNumberOrError(state, 2);

    moduleData.physics = (l_physics_PhysicsData*)lua_newuserdata(state, sizeof(l_physics_PhysicsData));
    moduleData.physics->physics = malloc(sizeof(physics_PhysicsData));

    physics_newSpace(moduleData.physics->physics, x, y);

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.physicsMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getSpaceGravity(lua_State* state)
{
    l_physics_PhysicsData* data = (l_physics_PhysicsData*)lua_touserdata(state, 1);

    cpVect grav = physics_getSpaceGravity(data->physics);

	lua_pushnumber(state, grav.x);
	lua_pushnumber(state, grav.y);

	return 2;
}

static int l_physics_updateSpace(lua_State* state)
{

    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

	//TODO add default value for dt.
	float dt = l_tools_toNumberOrError(state, 2);

    physics_updateSpace(physics->physics, dt);

	return 0;
}

static int l_physics_setSpaceDamping(lua_State* state)
{
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

	float damping = l_tools_toNumberOrError(state, 2);

    physics_setSpaceDamping(&physics->physics, damping);

	return 0;
}

static int l_physics_setSpaceIterations(lua_State* state)
{

    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

    int iterations = l_tools_toIntegerOrError(state, 2);

    physics_setSpaceIterations(physics->physics, iterations);

    return 0;
}

static int l_physics_setSpaceSleepTime(lua_State* state)
{

    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

    float sleep = l_tools_toNumberOrError(state, 2);

    physics_setSpaceSleepTime(physics->physics, sleep);

    return 0;
}


int l_physics_newBoxBody(lua_State* state)
{

    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    float mass = l_tools_toNumberOrError(state, 2);
    float width = l_tools_toNumberOrError(state, 3);
    float height = luaL_optnumber(state, 4, width);
    float moment = luaL_optnumber(state, 5, 0);
    const char* type = luaL_optstring(state, 6, "dynamic");

    moduleData.body = (l_physics_Body*)lua_newuserdata(state, sizeof(l_physics_Body));
    moduleData.body->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.body->physics = physics->physics;
    physics_newBoxBody(physics->physics, moduleData.body->body, mass, width, height, moment, type);


    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.bodyMT);
    lua_setmetatable(state, -2);

    return 1;
}

int l_physics_newCircleBody(lua_State* state)
{
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    float mass = l_tools_toNumberOrError(state, 2);
    float radius = l_tools_toNumberOrError(state, 3);
    float moment = l_tools_toNumberOrError(state, 4);
    cpVect offset = cpvzero;
    offset.x = l_tools_toNumberOrError(state, 5);
    offset.y = l_tools_toNumberOrError(state, 6);
    const char* type = luaL_optstring(state, 7, "dynamic");

    moduleData.body = (l_physics_Body*)lua_newuserdata(state, sizeof(l_physics_Body));
    moduleData.body->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.body->physics = physics->physics;

    physics_newCircleBody(physics->physics, moduleData.body->body, mass, radius, moment, offset, type);

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.bodyMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getBodyTorque(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getBodyTorque(body->body));

    return 1;
}

static int l_physics_getBodyCenterOfGravity(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = physics_getBodyCenterOfGravity(body->body);
    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_getBodyAngularVelocity(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getBodyAngularVelocity(body->body));

    return 1;
}

static int l_physics_getBodyForce(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = physics_getBodyForce(body->body);

    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_getBodyMass(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getBodyMass(body->body));

    return 1;
}

static int l_physics_getBodyAngle(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getBodyAngle(body->body));

    return 1;
}

static int l_physics_getBodyPosition(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = physics_getBodyPosition(body->body);

    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_setBodyAngle(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyAngle(body->body, value);

    return 0;
}

static int l_physics_setBodyAngularVelocity(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyAngularVelocity(body->body, value);

    return 0;
}

static int l_physics_setBodyCenterOfGravity(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyAngularVelocity(body->body, value);

    return 0;
}

static int l_physics_setBodyForce(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    physics_setBodyForce(body->body, vec);

    return 0;
}

static int l_physics_setBodyMass(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyMass(body->body, value);

    return 0;
}

static int l_physics_setBodyTorque(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setBodyTorque(body->body, value);

    return 0;
}

static int l_physics_setBodyVelocity(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    physics_setBodyVelocity(body->body, vec);

    return 0;
}

static int l_physics_setBodyPosition(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    physics_setBodyPosition(body->body, vec);

    return 0;
}

static int l_physics_setBodyMoment(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    physics_setBodyMoment(body->body, l_tools_toNumberOrError(state, 2));

	return 0;
}

int l_physics_newCircleShape(lua_State* state)
{
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 2);
    float radius = l_tools_toNumberOrError(state, 3);

    cpVect offset = cpvzero;
    offset.x = l_tools_toNumberOrError(state, 4);
    offset.y = l_tools_toNumberOrError(state, 5);

    moduleData.shape = (l_physics_Shape*)lua_newuserdata(state, sizeof(l_physics_Shape));
    moduleData.shape->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.shape->physics = physics->physics;

    physics_newCircleShape(physics->physics, body->body, moduleData.shape->shape, radius, offset);

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

int l_physics_newBoxShape(lua_State* state)
{
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 2);
    float width = l_tools_toNumberOrError(state, 3);
    float height = l_tools_toNumberOrError(state, 4);
    float radius = l_tools_toNumberOrError(state, 5);

    moduleData.shape = (l_physics_Shape*)lua_newuserdata(state, sizeof(l_physics_Shape));
    moduleData.shape->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.shape->physics = physics->physics;

    physics_newBoxShape(physics->physics, body->body, moduleData.shape->shape, width, height, radius);

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

int l_physics_newShape(lua_State* state)
{
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 2);
    float x1 = l_tools_toNumberOrError(state, 3);
    float y1 = l_tools_toNumberOrError(state, 4);
    float x2 = l_tools_toNumberOrError(state, 5);
    float y2 = l_tools_toNumberOrError(state, 6);
    float radius = l_tools_toNumberOrError(state, 7);

    moduleData.shape = (l_physics_Shape*)lua_newuserdata(state, sizeof(l_physics_Shape));

    moduleData.shape->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.shape->physics = physics->physics;
    physics_newShape(physics->physics, body->body, moduleData.shape->shape, x1, y1, x2, y2, radius);

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getShapeDensity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getShapeDensity(shape->shape));

    return 1;
}


static int l_physics_getShapeElasticity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getElasticity(shape->shape));

    return 1;
}

static int l_physics_getShapeFriction(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getFriction(shape->shape));

    return 1;
}

static int l_physics_getShapeMass(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getMass(shape->shape));

    return 1;
}

static int l_physics_getShapeMoment(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, physics_getMoment(shape->shape));

    return 1;
}

static int l_physics_setShapeFriction(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeFriction(shape->shape, value);

    return 0;
}

static int l_physics_setShapeDensity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeDensity(shape->shape, value);

    return 0;
}

static int l_physics_setShapeElasticity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeElasticity(shape->shape, value);

    return 0;
}

static int l_physics_setShapeMass(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    physics_setShapeMass(shape->shape, value);

    return 0;
}

static int l_physics_setShapeBody(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 2);

    physics_setShapeBody(shape->shape, body->body);

    return 0;
}

/* Free funcs */

static int l_physics_shapeGC(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);
    physics_shapeFree(shape->physics, shape->shape);

	return 0;
}

static int l_physics_bodyGC(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);
    physics_bodyFree(body->physics, body->body);

	return 0;
}

static int l_physics_gc(lua_State* state)
{
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    physics_free(physics->physics);

    return 0;
}

/* End free funcs */

static luaL_Reg const bodyMetatableFuncs[] =
{
    {"__gc",                         l_physics_bodyGC},
    {"getTorque",                    l_physics_getBodyTorque},
    {"getAngle",                     l_physics_getBodyAngle},
    {"getAngularVelocity",           l_physics_getBodyAngularVelocity},
    {"getCenterOfGravity",           l_physics_getBodyCenterOfGravity},
    {"getForce",                     l_physics_getBodyForce},
    {"getMass",                      l_physics_getBodyMass},
	{"getPosition",                  l_physics_getBodyPosition},
    //{"getBody",                      l_physics_gevBody},

    {"setAngle",                     l_physics_setBodyAngle},
    {"setAngularVelocity",           l_physics_setBodyAngularVelocity},
    {"setCenterOfGravity",           l_physics_setBodyCenterOfGravity},
    {"setForce",                     l_physics_setBodyForce},
    {"setMass",                      l_physics_setBodyMass},
    {"setPosition",                  l_physics_setBodyPosition},
    {"setToruqe",                    l_physics_setBodyTorque},
    {"setVelocity",                  l_physics_setBodyVelocity},

    {NULL, NULL}
};

static luaL_Reg const shapeMetatableFuncs[] =
{
    {"__gc",                         l_physics_shapeGC},
    {"getMoment",             		 l_physics_getShapeMoment},
	{"getFriction",           		 l_physics_getShapeFriction},
	{"getDensity",            		 l_physics_getShapeDensity},
	{"getElasticity",         		 l_physics_getShapeElasticity},
	{"getMass",               		 l_physics_getShapeMass},

	{"setFriction",           		 l_physics_setShapeFriction},
	{"setDensity",            		 l_physics_setShapeDensity},
	{"setElasticity",         		 l_physics_setShapeElasticity},
	{"setMass",               		 l_physics_setShapeMass},
	{"setBody",               		 l_physics_setShapeBody},

    {NULL, NULL}
};

static luaL_Reg const physicsSpaceMetatableFuncs[] =
{
    {"__gc",   l_physics_gc},
    {NULL, NULL}
};

static luaL_Reg const physicsFreeFuncs[] =
{
    {"setSpaceIterations",    l_physics_setSpaceIterations},
    {"setSpaceSleep",         l_physics_setSpaceSleepTime},
	{"setSpaceDamping",       l_physics_setSpaceDamping},
    {"newSpace",              l_physics_newSpace},
	{"update",                l_physics_updateSpace},
    {"getGravity",            l_physics_getSpaceGravity},
	{NULL, NULL}
};

static luaL_Reg const shapeRegFuncs[] =
{
	{"newBoxShape",           l_physics_newBoxShape},
    {"newCircleShape",        l_physics_newCircleShape},
    {"newShape",              l_physics_newShape},
	{NULL, NULL}
};

static luaL_Reg const bodyRegFuncs[] =
{
	{"newBoxBody",            l_physics_newBoxBody},
    {"newCircleBody",         l_physics_newCircleBody},
	{NULL, NULL}
};

int l_physics_register(lua_State *state)
{

    /* First register the new module */
    l_tools_registerModule(state, "physics", physicsFreeFuncs);
    moduleData.physicsMT = l_tools_makeTypeMetatable(state, physicsSpaceMetatableFuncs);

	/* Register new functions to this module */
    l_tools_registerFuncsInModule(state, "physics", bodyRegFuncs);
    moduleData.bodyMT = l_tools_makeTypeMetatable(state, bodyMetatableFuncs);

    l_tools_registerFuncsInModule(state, "physics", shapeRegFuncs);
    moduleData.shapeMT = l_tools_makeTypeMetatable(state, shapeMetatableFuncs);

    return 0;
}
