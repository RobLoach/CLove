/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../math/vector.h"
#include "../tools/utils.h"
#include "matrixstack.h"
#include <string.h>

#include <stdlib.h>
#include <stdio.h>

const static float stackSize = 32;

static struct {
  int head;
  mat4x4* stack;

  float rotX;
  float rotY;
  float rotZ;
  float rotAngle;

  float scaleX;
  float scaleY;
  float scaleZ;

} moduleData;

inline mat4x4* matrixstack_head() {
  return &moduleData.stack[moduleData.head];
}

void matrixstack_init(void) {
  moduleData.stack = malloc(sizeof(mat4x4) * stackSize);

  moduleData.rotAngle = 0;
  moduleData.rotX = 0;
  moduleData.rotY = 0;
  moduleData.rotZ = 0;

  moduleData.scaleX = 1;
  moduleData.scaleY = 1;
  moduleData.scaleZ = 1;

  moduleData.head = 0;
  m4x4_newIdentity(matrixstack_head());
}

void matrixstack_origin(void) {
  m4x4_newIdentity(matrixstack_head());
}

int matrixstack_push(void) {
  if(moduleData.head == stackSize - 1) {
      return 1;
    }

  memcpy(matrixstack_head() + 1, matrixstack_head(), sizeof(mat4x4));
  ++moduleData.head;
  return 0;
}

int matrixstack_pop(void) {
  if(moduleData.head == 0) {
      return 1;
    }

  --moduleData.head;
  return 0;
}

void matrixstack_translate(float x, float y, float z) {
  m4x4_translate(matrixstack_head(), x, y, z);
}

void matrixstack_scale(float x, float y, float z) {
  moduleData.scaleX = x;
  moduleData.scaleY = y;
  moduleData.scaleZ = z;
  m4x4_newRotationScale(matrixstack_head(), moduleData.rotAngle,
                        vec3_new(moduleData.rotX,moduleData.rotY,moduleData.rotZ),
                        moduleData.scaleX, moduleData.scaleY, moduleData.scaleZ);
}

void matrixstack_rotate(float a) {
  moduleData.rotAngle = a;
  m4x4_rotateZ(matrixstack_head(), moduleData.rotAngle, moduleData.scaleX, moduleData.scaleY, moduleData.scaleZ);
}

void matrixstack_rotate_3d(float r, float x, float y, float z) {
  moduleData.rotAngle = r;
  moduleData.rotX = x;
  moduleData.rotY = y;
  moduleData.rotZ = z;
  m4x4_newRotationScale(matrixstack_head(), r, vec3_new(x,y,z), moduleData.scaleX, moduleData.scaleY, moduleData.scaleZ);
}

void matrixstack_multiply(mat4x4 const* matrix) {
  mat4x4 m;
  memcpy(&m, matrixstack_head(), sizeof(mat4x4));
  m4x4_mulM4x4(matrixstack_head(), matrix, &m);
}

