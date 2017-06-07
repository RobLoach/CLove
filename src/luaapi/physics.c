/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "physics.h"

/*
 * For more info and docs see:
 * https://chipmunk-physics.net/release/ChipmunkLatest-Docs/
 */

/*
 * TODO:
 * Add POLYGON
 */

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

    moduleData.physics->physics->space = cpSpaceNew();
    moduleData.physics->physics->gravity = cpv(x, y);
    cpSpaceSetGravity(moduleData.physics->physics->space, moduleData.physics->physics->gravity);


    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.physicsMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getSpaceGravity(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_physics_PhysicsData* data = (l_physics_PhysicsData*)lua_touserdata(state, 1);

    cpVect grav = cpSpaceGetGravity(data->physics->space);

	lua_pushnumber(state, grav.x);
	lua_pushnumber(state, grav.y);

	return 2;
}

static int l_physics_updateSpace(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

    float dt = luaL_optnumber(state, 2, timer_getDelta());

    cpSpaceStep(physics->physics->space, dt);

	return 0;
}

static int l_physics_setSpaceDamping(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

	float damping = l_tools_toNumberOrError(state, 2);

    cpSpaceSetDamping(physics->physics->space, damping);

	return 0;
}

static int l_physics_setSpaceIterations(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

    int iterations = l_tools_toIntegerOrError(state, 2);

    cpSpaceSetIterations(physics->physics->space, iterations);

    return 0;
}

static int l_physics_setSpaceSleepTime(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);

    float sleep = l_tools_toNumberOrError(state, 2);

    cpSpaceSetSleepTimeThreshold(physics->physics->space, sleep);

    return 0;
}


int l_physics_newBoxBody(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    const char* type = l_tools_toStringOrErrorPlusMsg(state, 2, "You must provide a type, eg: dynamic,static,kinematic");
    float width = l_tools_toNumberOrErrorPlusMsg(state, 3, "You must provide a width");
    float height = luaL_optnumber(state, 4, width);
    float mass = luaL_optnumber(state, 5, 1.0f);
    float moment = luaL_optnumber(state, 6, 0.0f);

    moduleData.body = (l_physics_Body*)lua_newuserdata(state, sizeof(l_physics_Body));
    moduleData.body->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.body->physics = physics->physics;

    cpFloat _moment = moment;

    // If we don't provide a default moment then we let chipmunk calculate one
    if (_moment == 0)
        _moment = cpMomentForBox(mass, width, height);

    if (strcmp(type, "dynamic") == 0)
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNewDynamic());
    else if (strcmp(type, "static") == 0)
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNewStatic());
    else if (strcmp(type, "kinematic") == 0)
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNewKinematic());
    else
    {
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNew(mass, _moment));
        /*
        const char* err = util_concatenate("Undefined type: ", type);
        l_tools_trowError(state, err);
        return -1;
        */
    }

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.bodyMT);
    lua_setmetatable(state, -2);

    return 1;
}

int l_physics_newCircleBody(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    const char* type = luaL_optstring(state, 2, "dynamic");
    float outer_radius = luaL_optnumber(state, 3, 1.0f);
    cpVect offset = cpvzero;
    offset.x = luaL_optnumber(state, 4, 0.0f);
    offset.y = luaL_optnumber(state, 5, 0.0f);
    float mass = luaL_optnumber(state, 6, 1.0f);
    float moment = luaL_optnumber(state, 7, 0.0f);
    float inner_radius = luaL_optnumber(state, 8, 0.0f);

    moduleData.body = (l_physics_Body*)lua_newuserdata(state, sizeof(l_physics_Body));
    moduleData.body->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.body->physics = physics->physics;

    cpFloat _moment = moment;

    if (_moment == 0)
        _moment = cpMomentForCircle(mass, inner_radius, outer_radius, offset);

    if (strcmp(type, "dynamic") == 0)
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNewDynamic());
    else if (strcmp(type, "kinematic") == 0)
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNewKinematic());
    else if (strcmp(type, "static") == 0)
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNewStatic());
    else
    {
        moduleData.body->body = cpSpaceAddBody(physics->physics->space, cpBodyNew(mass, _moment));
        /*
        const char* err = util_concatenate("Undefined type: ", type);
        l_tools_trowError(state, err);
        return -1;
        */
    }

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.bodyMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getBodyMoment(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpBodyGetMoment(body->body));

    return 1;
}

static int l_physics_getBodyTorque(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpBodyGetTorque(body->body));

    return 1;
}

static int l_physics_getBodyCenterOfGravity(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpBodyGetCenterOfGravity(body->body);
    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_getBodyAngularVelocity(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpBodyGetAngularVelocity(body->body));

    return 1;
}

static int l_physics_getBodyForce(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpBodyGetForce(body->body);

    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_getBodyMass(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpBodyGetMass(body->body));

    return 1;
}

static int l_physics_getBodyAngle(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpBodyGetAngle(body->body));

    return 1;
}

static int l_physics_getBodyPosition(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpBodyGetPosition(body->body);

    lua_pushnumber(state, vec.x);
    lua_pushnumber(state, vec.y);

    return 2;
}

static int l_physics_isBodySleeping(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    lua_pushboolean(state, cpBodyIsSleeping(body->body));

    return 1;
}

static int l_physics_getBodyType(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpBodyType type = cpBodyGetType(body->body);

    const char* s_type;

    if (type == CP_BODY_TYPE_STATIC)
        s_type = "static";
    else if (type == CP_BODY_TYPE_DYNAMIC)
        s_type = "dynamic";
    else if (type == CP_BODY_TYPE_KINEMATIC)
        s_type = "kinematic";
    else
        s_type = "unknown";

    lua_pushstring(state, s_type);

    return 1;
}

/* Forces a body to sleep. Cannot be called from a callback! */
static int l_physics_setBodySleep(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpBodySleep(body->body);

    return 0;
}

static int l_physics_setBodyAngle(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpBodySetAngle(body->body, value);

    return 0;
}

static int l_physics_setBodyAngularVelocity(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpBodySetAngularVelocity(body->body, value);

    return 0;
}

static int l_physics_setBodyCenterOfGravity(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect value = cpvzero;
    value.x = l_tools_toNumberOrError(state, 2);
    value.y = l_tools_toNumberOrError(state, 3);

    cpBodySetCenterOfGravity(body->body, value);

    return 0;
}

static int l_physics_setBodyForce(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    cpBodySetForce(body->body, vec);

    return 0;
}

/*
 * The moment is like the rotational mass of a body
 * Note: !This function may also set the mass of the object!
 */
static int l_physics_setBodyMoment(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    /*
     * You can provide a custom moment
     * for this body *or* you can choose what
     * type of body this is and let chipmunk
     * calculate the value based on some attributes
     */

    int index = 2;

    cpFloat _mass = 0;
    cpFloat moment = 0;
    if (lua_type(state, 2) == LUA_TNUMBER)
    {
        moment = l_tools_toNumberOrError(state, 2);
    }
    else if (lua_type(state, 2) == LUA_TSTRING)
    {
        const char* whatFor = lua_tostring(state, 2);
        if (strcmp(whatFor, "box") == 0)
        {
            cpFloat mass = l_tools_toNumberOrError(state, 3);
            cpFloat width = l_tools_toNumberOrError(state, 4);
            cpFloat height = l_tools_toNumberOrError(state, 5);
            _mass = mass;
            moment = cpMomentForBox(mass, width, height);
        }
        else if (strcmp(whatFor, "circle") == 0)
        {
            cpFloat mass = l_tools_toNumberOrError(state, index++);
            cpFloat inner_radius = l_tools_toNumberOrError(state, index++);
            cpFloat outer_radius = l_tools_toNumberOrError(state, index++);
            cpVect offset = cpvzero;
            offset.x = l_tools_toNumberOrError(state, index++);
            offset.y = l_tools_toNumberOrError(state, index++);
            _mass = mass;
            moment = cpMomentForCircle(mass, inner_radius, outer_radius, offset);
        }
        else if (strcmp(whatFor, "segment") == 0)
        {

            cpFloat mass = l_tools_toNumberOrError(state, index++);

            cpVect a = cpvzero;
            a.x = l_tools_toNumberOrError(state, index++);
            a.y = l_tools_toNumberOrError(state, index++);
            cpVect b = cpvzero;
            b.x = l_tools_toNumberOrError(state, index++);
            b.y = l_tools_toNumberOrError(state, index++);

            cpFloat radius = l_tools_toNumberOrError(state, index++);
            _mass = mass;
            moment == cpMomentForSegment(mass, a, b, radius);
        }
    }

    if (_mass > 0)
        cpBodySetMass(body->body, _mass);

    cpBodySetMoment(body->body, moment);

    return 0;
}

static int l_physics_setBodyMass(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpBodySetMass(body->body, value);

    return 0;
}

static int l_physics_setBodyTorque(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpBodySetTorque(body->body, value);

    return 0;
}

static int l_physics_setBodyVelocity(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 2);
    vec.y = l_tools_toNumberOrError(state, 3);

    cpBodySetVelocity(body->body, vec);

    return 0;
}

static int l_physics_setBodyPosition(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a body");
    l_tools_checkUserDataPlusErrMsg(state, 2, "You must provide a space");

    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);
    l_physics_PhysicsData* space = (l_physics_PhysicsData*)lua_touserdata(state, 2);

    cpVect vec = cpvzero;
    vec.x = l_tools_toNumberOrError(state, 3);
    vec.y = l_tools_toNumberOrError(state, 4);

    cpBodySetPosition(body->body, vec);
    /*
     * Docs:
     * When changing the position you may also want to call cpSpaceReindexShapesForBody()
     * to update the collision detection information for the attached shapes if plan to
     * make any queries against the space.
     */
    cpSpaceReindexShapesForBody(space->physics->space, body->body);

    return 0;
}

int l_physics_newCircleShape(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space!");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    l_tools_checkUserDataPlusErrMsg(state, 2, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 2);

    float radius = l_tools_toNumberOrError(state, 3);
    cpVect offset = cpvzero;
    offset.x = l_tools_toNumberOrError(state, 4);
    offset.y = l_tools_toNumberOrError(state, 5);

    moduleData.shape = (l_physics_Shape*)lua_newuserdata(state, sizeof(l_physics_Shape));
    moduleData.shape->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.shape->physics = physics->physics;

    moduleData.shape->shape = cpSpaceAddShape(physics->physics->space, cpCircleShapeNew(body->body, radius, offset));

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);


    return 1;
}

int l_physics_newBoxShape(lua_State* state)
{
    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space!");
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    l_tools_checkUserDataPlusErrMsg(state, 2, "You must provide a body");
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 2);

    float width = l_tools_toNumberOrError(state, 3);
    float height = luaL_optnumber(state, 4, width);
    float radius = luaL_optnumber(state, 5, 0);
    float offset_x = luaL_optnumber(state, 6, 0);
    float offset_y = luaL_optnumber(state, 7, 0);

    moduleData.shape = (l_physics_Shape*)lua_newuserdata(state, sizeof(l_physics_Shape));
    moduleData.shape->physics = malloc(sizeof(physics_PhysicsData));
    moduleData.shape->physics = physics->physics;

    if (offset_x == 0 && offset_y == 0)
        // No offset is needed
        moduleData.shape->shape = cpSpaceAddShape(physics->physics->space, cpBoxShapeNew(body->body, width, height, radius));
    else
    {
        //Apply offset to shape!

        cpVect off = cpv(offset_x, offset_y);
        cpBB bb = cpBBNewForExtents(off, width * 0.5f, height * 0.5f);

        moduleData.shape->shape = cpSpaceAddShape(physics->physics->space, cpBoxShapeNew2(body->body, bb, radius));

    }

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

int l_physics_newShape(lua_State* state)
{

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a space");
    l_tools_checkUserDataPlusErrMsg(state, 2, "You must provide a body");

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

    moduleData.shape->shape = cpSpaceAddShape(physics->physics->space, cpSegmentShapeNew(body->body, cpv(x1, y1), cpv(x2, y2), radius));

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shapeMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_physics_getShapeDensity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpShapeGetDensity(shape->shape));

    return 1;
}


static int l_physics_getShapeElasticity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpShapeGetElasticity(shape->shape));

    return 1;
}

static int l_physics_getShapeFriction(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpShapeGetFriction(shape->shape));

    return 1;
}

static int l_physics_getShapeMass(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpShapeGetMass(shape->shape));

    return 1;
}

static int l_physics_getShapeMoment(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    lua_pushnumber(state, cpShapeGetMass(shape->shape));

    return 1;
}

//TODO look into these
//cpShapeSetCollisionType
static int l_physics_setShapeFilter(lua_State* state)
{

    cpShapeFilter shapeFilter;

    l_tools_checkUserDataPlusErrMsg(state, 1, "You must provide a shape");
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    /*
     * Two objects with the same non-zero group value do not collide.
     */
    uint32_t group = l_tools_toIntegerOrError(state, 2);
    /*
     * A bitmask of user definable categories that this object belongs to.
     * The category/mask combinations of both objects in a collision must agree
     * for a collision to occur.
     */
    uint32_t categories = l_tools_toIntegerOrError(state, 3);
    /*
     * A bitmask of user definable category types that this object object collides with.
     * The category/mask combinations of both objects in a collision must agree for a collision
     * to occur.
     */
    uint32_t mask = l_tools_toIntegerOrError(state, 4);

    shapeFilter.group = group;
    shapeFilter.categories = categories;
    shapeFilter.mask = mask;
    cpShapeSetFilter(shape->shape, shapeFilter);

    return 0;
}

static int l_physics_setShapeFriction(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpShapeSetFriction(shape->shape, value);

    return 0;
}

static int l_physics_setShapeDensity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpShapeSetDensity(shape->shape, value);

    return 0;
}

static int l_physics_setShapeElasticity(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpShapeSetElasticity(shape->shape, value);

    return 0;
}

static int l_physics_setShapeMass(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    float value = l_tools_toNumberOrError(state, 2);

    cpShapeSetMass(shape->shape, value);

    return 0;
}

static int l_physics_setShapeBody(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 2);

    if (shape->physics && shape->shape && body->body)
        cpShapeSetBody(shape->shape, body->body);

    return 0;
}

/* Free funcs */

static int l_physics_shapeGC(lua_State* state)
{
    l_physics_Shape* shape = (l_physics_Shape*)lua_touserdata(state, 1);

    if (shape->physics && shape->shape)
        cpSpaceRemoveShape(shape->physics->space, shape->shape);

	return 0;
}

static int l_physics_bodyGC(lua_State* state)
{
    l_physics_Body* body = (l_physics_Body*)lua_touserdata(state, 1);
    cpSpaceRemoveBody(body->physics->space, body->body);

	return 0;
}

static int l_physics_gc(lua_State* state)
{
    l_physics_PhysicsData* physics = (l_physics_PhysicsData*)lua_touserdata(state, 1);
    cpSpaceFree(physics->physics->space);

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
    {"getMoment",                    l_physics_getBodyMoment},
	{"getPosition",                  l_physics_getBodyPosition},
    {"getType",                      l_physics_getBodyType},
    {"isSleeping",                   l_physics_isBodySleeping},
    //{"getBody",                      l_physics_gevBody},

    {"setSleep",                     l_physics_setBodySleep},
    {"setMoment",                    l_physics_setBodyMoment},
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

    {"setFilter",                    l_physics_setShapeFilter},
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
