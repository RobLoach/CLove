/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../3rdparty/chipmunk/include/chipmunk/chipmunk.h"

typedef struct
{
    cpSpace* space;

    cpVect gravity;

} physics_PhysicsData;

// Space related functions go here
void physics_newSpace(physics_PhysicsData* physics, cpFloat grav_x, cpFloat grav_y);
void physics_setSpaceIterations(physics_PhysicsData* physics, int iterations);
void physics_setSpaceSleepTime(physics_PhysicsData* physics, cpFloat sleep);
void physics_Spaceupdate(physics_PhysicsData* physics, float dt);
void physics_setSpaceDaping(physics_PhysicsData* physics, cpFloat damping);

// Body stuff goes here
void physics_newCircleBody(physics_PhysicsData* physics, cpFloat mass, cpFloat radius, cpFloat moment, cpVect offset, char* type);
void physics_setBodyPosition(cpBody* body, cpFloat x, cpFloat y);
cpFloat physics_getBodyAngle(cpBody* body);
cpFloat physics_getBodyMass(cpBody* body);
cpVect physics_getBodyForce(cpBody* body);
cpFloat physics_getBodyTorque(cpBody* body);
cpVect physics_getBodyCenterOfGravity(cpBody* body);
cpFloat physics_getBodyAngularVelocity(cpBody* body);
cpVect physics_getBodyForce(cpBody* body);

void physics_setBodyAngle(cpBody* body, cpFloat angle);
void physics_setBodyAngularVelocity(cpBody* body, cpFloat angular);
void physics_setCenterOfGravity(cpBody* body, cpVect center);
void physics_setForce(cpBody* body, cpFloat force);
void physics_setMass(cpBody* body, cpFloat mass);
void physics_setTorque(cpBody* body, cpFloat torque);
void physics_setVelocity(cpBody* body, cpVect velocity);

//Shape stuff goes here
void physics_newCircleShape(physics_PhysicsData* physics, cpBody* body, cpFloat radius, cpVect offset);

cpFloat physics_getShapeDensity(cpShape* shape);
cpFloat physics_getElasticity(cpShape* shape);
cpFloat physics_getFriction(cpShape* shape);
cpFloat physics_getMass(cpShape* shape);
cpFloat physics_getMoment(cpShape* shape);

void physics_setShapeDensity(cpShape* shape, cpFloat density);
void physics_setShapeElasticity(cpShape* shape, cpFloat elasticity);
void physics_setShapeFriction(cpShape* shape, cpFloat friction);
void physics_setShapeMass(cpShape* shape, cpFloat mass);
void physics_setShapeBody(cpShape* shape, cpBody* body);









