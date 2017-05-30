/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "../3rdparty/CMath/cmath.h"


void m4x4_newTransform3d(mat4x4 *out, vec3 pos, float angle, vec3 axis, vec3 scale,
                         float ox, float oy, float kx, float ky);
void m4x4_newTransform2d(mat4x4 *out, float x, float y, float r, float sx, float sy,
                      float ox, float oy, float kx, float ky);

void m4x4_translate(mat4x4 *inout, float x, float y, float z);

void m4x4_scalexyz(mat4x4 *inout, float x, float y, float z);
void m4x4_scale(mat4x4 *inout,float angle, float r_x, float r_y, float r_z, float x, float y, float z);
void m4x4_rotateZ(mat4x4 *inout, float a, float s_x, float s_y, float s_z) ;
void m4x4_rotateY(mat4x4 *inout, float a, float s_x, float s_y, float s_z) ;
void m4x4_rotateX(mat4x4 *inout, float a, float s_x, float s_y, float s_z) ;

void m4x4_shear2d(mat4x4 *inout, float x, float y);

void m3x3_newTransform2d(mat3x3 *out, float x, float y, float r, float sx, float sy,
                         float ox, float oy, float kx, float ky, float w, float h);

// Assumes that v is a 2d point (homogeneous coord == 1).
// This allows much faster multiplication
void m3x3_mulV2(vec2 *out, mat3x3 const* m, vec2 const* v);
