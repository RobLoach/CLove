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

static struct {
    cpBody** bodies;
    int bodies_size;
    cpShape** shapes;
    int shapes_size;
} moduleData;

static void grow_bodies(cpBody* body)
{
    if (moduleData.bodies_size == 0)
    {
        moduleData.bodies = malloc(sizeof(cpBody));
        moduleData.bodies_size = 1;
    }
    else
        moduleData.bodies = realloc(moduleData.bodies, sizeof(cpBody) * moduleData.bodies_size);

    moduleData.bodies[moduleData.bodies_size] = body;
    moduleData.bodies_size ++;

}

static void grow_shapes(cpShape* shape)
{
    if (moduleData.shapes_size == 0)
    {
        moduleData.shapes = malloc(sizeof(cpShape));
        moduleData.shapes_size = 1;
    }
    else
        moduleData.shapes = realloc(moduleData.shapes, sizeof(cpShape) * moduleData.shapes_size);

    moduleData.shapes[moduleData.shapes_size] = shape;
    moduleData.shapes_size ++;
}

void physics_newSpace(physics_PhysicsData* physics, cpFloat grav_x, cpFloat grav_y)
{
    physics->space = cpSpaceNew();
    physics->gravity = cpv(grav_x, grav_y);
    cpSpaceSetGravity(physics->space, physics->gravity);
}

void physics_setSpaceIterations(physics_PhysicsData* physics, int iterations)
{
    cpSpaceSetIterations(physics->space, iterations);
}

void physics_setSpaceSleepTime(physics_PhysicsData* physics, cpFloat sleep)
{
    cpSpaceSetSleepTimeThreshold(physics->space, sleep);
}

void physics_updateSpace(physics_PhysicsData* physics, cpFloat dt)
{
    cpSpaceStep(physics->space, dt);
}

void physics_setSpaceDaping(physics_PhysicsData* physics, cpFloat damping)
{
    cpSpaceSetDamping(physics->space, damping);
}

/*********** Body stuff goes here ****************/

void physics_newBoxBody(physics_PhysicsData* physics, cpFloat mass, cpFloat width, cpFloat height, cpFloat moment, const char* type)
{
    cpBody* body;
    cpFloat _moment = 0;

    // If we don't provide a default moment then we let chipmunk calculate one
    if (moment)
        _moment = cpMomentForBox(mass, width, height);

    if (strcmp(type, "static"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNewStatic());
    }
    else if (strcmp(type, "dynamic"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNew(mass, moment == 0 ? _moment : moment));
    }
    else
        printf("%s %s %s \n", "Error, type: ", type, " is not acceptable");

    grow_bodies(body);

}

void physics_newCircleBody(physics_PhysicsData* physics, cpFloat mass, cpFloat radius, cpFloat moment, cpVect offset, const char *type)
{

    cpFloat _moment = 0;

    // If we don't provide a default moment then we let chipmunk calculate one
    if (moment == 0)
        cpMomentForCircle(mass, 0, radius, offset);

    cpBody* body;
    if (strcmp(type, "static"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNewStatic());
    }
    else if (strcmp(type, "dynamic"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNew(mass, moment == 0 ? _moment : moment));
    }
    else
        printf("%s %s %s \n", "Error, type: ", type, " is not acceptable");

    grow_bodies(body);

}

cpFloat physics_getBodyTorque(cpBody* body)
{
    return cpBodyGetTorque(body);
}

cpVect physics_getBodyCenterOfGravity(cpBody* body)
{
    return cpBodyGetCenterOfGravity(body);
}

cpFloat physics_getBodyAngularVelocity(cpBody* body)
{
    return cpBodyGetAngularVelocity(body);
}

cpVect physics_getBodyForce(cpBody* body)
{
    return cpBodyGetForce(body);
}

cpFloat physics_getBodyMass(cpBody* body)
{
    return cpBodyGetMass(body);
}

cpFloat physics_getBodyAngle(cpBody* body)
{
    return cpBodyGetAngle(body);
}

cpVect physics_getBodyPosition(cpBody* body)
{
    return cpBodyGetPosition(body);
}


void physics_setBodyAngle(cpBody* body, cpFloat angle)
{
    cpBodySetAngle(body, angle);
}

void physics_setBodyAngularVelocity(cpBody* body, cpFloat angular)
{
    cpBodySetAngularVelocity(body, angular);
}

void physics_setBodyCenterOfGravity(cpBody* body, cpVect center)
{
    cpBodySetCenterOfGravity(body, center);
}

void physics_setBodyForce(cpBody* body, cpVect force)
{
    cpBodySetForce(body, force);
}

void physics_setBodyMass(cpBody* body, cpFloat mass)
{
    cpBodySetMass(body, mass);
}

void physics_setBodyTorque(cpBody* body, cpFloat torque)
{
    cpBodySetTorque(body, torque);
}

void physics_setBodyVelocity(cpBody* body, cpVect velocity)
{
    cpBodySetVelocity(body, velocity);
}

void physics_setBodyPosition(cpBody* body, cpVect position)
{
    cpBodySetPosition(body, position);
}

/*********** End of Body stuff ****************/


/*********** Shape stuff goes here ****************/
void physics_newCircleShape(physics_PhysicsData* physics, cpBody* body, cpFloat radius, cpVect offset)
{
    cpShape* shape = cpSpaceAddShape(physics->space, cpCircleShapeNew(body, radius, offset));

    grow_shapes(shape);
}

void physics_newBoxShape(physics_PhysicsData* physics, cpBody* body, cpFloat width, cpFloat height, cpFloat radius)
{
    cpShape* shape = cpSpaceAddShape(physics->space, cpBoxShapeNew(body, width, height, radius));

    grow_shapes(shape);
}

void physics_newShape(physics_PhysicsData* physics, cpBody* body, cpFloat x1, cpFloat y1, cpFloat x2, cpFloat y2, cpFloat radius)
{
    cpShape* shape = cpSpaceAddShape(physics->space, cpSegmentShapeNew(body, cpv(x1, y1), cpv(x2, y2), radius));

    grow_shapes(shape);
}

cpFloat physics_getShapeDensity(cpShape* shape)
{
    return cpShapeGetDensity(shape);
}

cpFloat physics_getElasticity(cpShape* shape)
{
    return cpShapeGetElasticity(shape);
}

cpFloat physics_getFriction(cpShape* shape)
{
    return cpShapeGetFriction(shape);
}

cpFloat physics_getMass(cpShape* shape)
{
    return cpShapeGetMass(shape);
}

cpFloat physics_getMoment(cpShape* shape)
{
    return cpShapeGetMass(shape);
}

void physics_setShapeFriction(cpShape* shape, cpFloat v)
{
    cpShapeSetFriction(shape, v);
}

void physics_setShapeDensity(cpShape* shape, cpFloat density)
{
    cpShapeSetDensity(shape, density);
}

void physics_setShapeElasticity(cpShape* shape, cpFloat elasticity)
{
    cpShapeSetElasticity(shape, elasticity);
}

void physics_setShapeMass(cpShape* shape, cpFloat mass)
{
    cpShapeSetMass(shape, mass);
}

void physics_setShapeBody(cpShape* shape, cpBody* body)
{
    cpShapeSetBody(shape, body);
}

/*********** End of Shape stuff ****************/


/*********** OTHER ***********************/

void physics_free(physics_PhysicsData* physics)
{
    for (int i = 0; i < moduleData.bodies_size; i++)
        cpSpaceRemoveBody(physics->space, moduleData.bodies[i]);

    for (int i = 0; i < moduleData.shapes_size; i++)
        cpSpaceRemoveShape(physics->space, moduleData.shapes[i]);

    moduleData.bodies_size = 0;
    moduleData.shapes_size = 0;

    cpSpaceFree(physics->space);
}













