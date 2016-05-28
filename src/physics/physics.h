/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once 
#include "../3rdparty/chipmunk/include/chipmunk/chipmunk.h"

typedef struct {
  cpBody* body;
  float mass;
}physics_Body;

typedef struct {
  cpShape* shape;
}physics_Shape;

typedef struct { 
  cpSpace* space;
  cpVect gravity;
}physics_World;

void physics_init();

//scene functions
void physics_newWorld(physics_World* world, float x, float y, float sleep);
void physics_setIterations(int number);
void physics_updateWorld(physics_World* world, float dt);

//body functions
void physics_newBody(physics_World* world, physics_Body* body, float mass, float momentum);

//position
void physics_setBodyPosition(physics_Body* body, float x, float y);
float physics_getBodyX(physics_Body* body);
float physics_getBodyY(physics_Body* body);

float physics_getAngle(physics_Body* body);
void physics_setAngle(physics_Body* body, float angle);

//velocity
void physics_setBodyVel(physics_Body* body, float x, float y);

float physics_getBodyVelX(physics_Body* body);
float physics_getBodyVelY(physics_Body* body);

//momentu
void physics_setBodyMomentum(physics_Body* body, float momentum);

//garbage collection
void physics_freeBody(physics_Body* body);
void physics_freeWorld(physics_World* world);
void physics_freeShape(physics_Shape* shape);

