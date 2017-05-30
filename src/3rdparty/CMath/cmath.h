/*
#   CMath
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include <stdbool.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
  float x;
  float y;
  float z;
  float w;
} vec4;

typedef struct {
  float x;
  float y;
  float z;
} vec3;

vec3 vec3_new(float x, float y, float z);
vec3 vec3_newv(vec3 a);
vec3 v3_add   (vec3 a, vec3 b);
vec3 v3_adds  (vec3 a, float s);
vec3 v3_sub   (vec3 a, vec3 b);
vec3 v3_subs  (vec3 a, float s);
vec3 v3_mul   (vec3 a, vec3 b);
vec3 v3_muls  (vec3 a, float s);
vec3 v3_div   (vec3 a, vec3 b);
vec3 v3_divs  (vec3 a, float s);
vec3 v3_norm  (vec3 v);
vec3 v3_proj  (vec3 v, vec3 onto);
vec3 v3_cross (vec3 a, vec3 b);
float v3_angle_between(vec3 a, vec3 b);
float  v3_dot   (vec3 a, vec3 b);
float  v3_length(vec3 v);

typedef struct {
  float x;
  float y;
} vec2;

vec2 vec2_new (float x, float y);
vec2 v2_add   (vec2 a, vec2 b);
vec2 v2_adds  (vec2 a, float s);
vec2 v2_sub   (vec2 a, vec2 b);
vec2 v2_subs  (vec2 a, float s);
vec2 v2_mul   (vec2 a, vec2 b);
vec2 v2_muls  (vec2 a, float s);
vec2 v2_div   (vec2 a, vec2 b);
vec2 v2_divs  (vec2 a, float s);
vec2 v2_norm   (vec2 v);
float v2_dot   (vec2 a, vec2 b);
float v2_length(vec2 v);

typedef struct {
  float m[4][4];
} mat4x4;

mat4x4 m4x4_new(float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33);
void m4x4_set   (mat4x4 *out,
                 float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33);

typedef struct {
  float m[3][3];
} mat3x3;

void m4x4_newIdentity(mat4x4 *out);
void m4x4_newOrtho(mat4x4 *projectionMatrix, float left, float right, float bottom, float top, float back, float front);
void m4x4_newPerspective(mat4x4* projectionMatrix, float fov, float ratio, float zNear, float zFar);
void m4x4_newLookAt(mat4x4* in, vec3 pos, vec3 target, vec3 up);
void m4x4_newScaling(mat4x4 *out, float x, float y, float z);
void m4x4_newTranslation(mat4x4 *out, float x, float y, float z);
void m4x4_newRotationX(mat4x4 *out, float a);
void m4x4_newRotationY(mat4x4 *out, float a);
void m4x4_newRotationZ(mat4x4 *out, float a);
void m4x4_newRotationScale(mat4x4 *out, float a, vec3 axis, float s_x, float s_y, float s_z);
void m4x4_newRotation(mat4x4 *out, float a, vec3 axis);
void m4x4_newTransform2d(mat4x4 *out, float x, float y, float r, float sx, float sy,
                         float ox, float oy, float kx, float ky);
void m4x4_mulM4x4(mat4x4 *out, mat4x4 const* a, mat4x4 const* b);
