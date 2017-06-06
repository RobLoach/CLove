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
} moduleData;

void physics_newSpace(physics_PhysicsData* physics, cpFloat grav_x, cpFloat grav_y)
{
    physics->space = cpSpaceNew();
    physics->gravity = cpv(grav_x, grav_y);
    cpSpaceSetGravity(physics->space, cpv(grav_x, grav_y));

    printf("%s \n", "Called newSpace");

}

cpVect physics_getSpaceGravity(physics_PhysicsData* physics)
{
	return cpSpaceGetGravity(physics->space);
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
    printf("%s \n", "Called updateSpace - 1");
    cpSpaceStep(physics->space, dt);
    printf("%s \n", "Called updateSpace - 2");
}

void physics_setSpaceDamping(physics_PhysicsData* physics, cpFloat damping)
{
    cpSpaceSetDamping(physics->space, damping);
}

/*********** Body stuff goes here ****************/

void physics_newBoxBody(physics_PhysicsData* physics, cpBody* body, cpFloat mass, cpFloat width, cpFloat height, cpFloat moment, const char* type)
{
    cpFloat _moment = 0;

    // If we don't provide a default moment then we let chipmunk calculate one
    if (moment == 0)
        _moment = cpMomentForBox(mass, width, height);
	else
		_moment = moment;

    printf("%s \n", "Called newBoxBody - 1");

    /*
    if (strcmp(type, "static"))
    {
        body = cpSpaceAddBody(physics->space, cpBodyNewStatic());
    }
    else if (strcmp(type, "dynamic"))
    {
    */
        printf("Called \n");
        body = cpSpaceAddBody(physics->space, cpBodyNew(mass, _moment));
    /*}
    else
        printf("%s %s %s \n", "Error, type: ", type, " is not acceptable");
        */

    printf("%s \n", "Called newBoxBody - 2");

}

void physics_newCircleBody(physics_PhysicsData* physics, cpBody* body, cpFloat mass, cpFloat radius, cpFloat moment, cpVect offset, const char *type)
{

    cpFloat _moment = 0;

    // If we don't provide a default moment then we let chipmunk calculate one
    if (moment == 0)
        cpMomentForCircle(mass, 0, radius, offset);

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

void physics_setBodyMoment(cpBody* body, cpFloat moment)
{
	cpBodySetMoment(body, moment);
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
void physics_newCircleShape(physics_PhysicsData* physics, cpBody* body, cpShape* shape, cpFloat radius, cpVect offset)
{
    shape = cpSpaceAddShape(physics->space, cpCircleShapeNew(body, radius, offset));
}

void physics_newBoxShape(physics_PhysicsData* physics, cpBody* body, cpShape* shape, cpFloat width, cpFloat height, cpFloat radius)
{
    printf("%s \n", "Called newBoxShape - 1");
    shape = cpSpaceAddShape(physics->space, cpBoxShapeNew(body, width, height, radius));
    printf("%s \n", "Called newBoxShape - 2");
}

void physics_newShape(physics_PhysicsData* physics, cpBody* body, cpShape* shape, cpFloat x1, cpFloat y1, cpFloat x2, cpFloat y2, cpFloat radius)
{
    shape = cpSpaceAddShape(physics->space, cpSegmentShapeNew(body, cpv(x1, y1), cpv(x2, y2), radius));
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


void physics_bodyFree(physics_PhysicsData* data, cpBody* body)
{
	cpSpaceRemoveBody(data->space, body);
}

void physics_shapeFree(physics_PhysicsData* data, cpShape* shape)
{
	cpSpaceRemoveShape(data->space, shape);
}

void physics_free(physics_PhysicsData* physics)
{
    cpSpaceFree(physics->space);
    free(physics);
}













