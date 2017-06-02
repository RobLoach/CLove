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

void physics_newBoxBody(physics_PhysicsData* physics, cpFloat mass, cpFloat width, cpFloat height, cpFloat moment, char* type)
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

}

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
    }
    else if (strcmp(type, "dynamic"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNew(mass, moment == 0 ? _moment : moment));
    }
    else
        printf("%s %s %s \n", "Error, type: ", type, " is not acceptable");
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
    cpVect pos = cpBodyGetPosition(body);
    return pos;
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

void physics_setBodyPosition(cpBody* body, cpFloat x, cpFloat y)
{
    cpBodySetPosition(body, cpv(x, y));
}

/*********** End of Body stuff ****************/


/*********** Shape stuff goes here ****************/
void physics_newCircleShape(physics_PhysicsData* physics, cpBody* body, cpFloat radius, cpVect offset)
{
    cpSpaceAddShape(physics->space, cpCircleShapeNew(body, radius, offset));
}

void physics_newBoxShape(physics_PhysicsData* physics, cpBody* body, cpFloat width, cpFloat height, cpFloat radius)
{
    cpSpaceAddShape(physics->space, cpBoxShapeNew(body, width, height, radius));
}

void physics_setShapeFriction(cpShape* shape, cpFloat v)
{
    cpShapeSetFriction(shape, v);
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
















