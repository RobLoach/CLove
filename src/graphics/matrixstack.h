/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../math/vector.h"

void matrixstack_init(void);
int matrixstack_push(void);
int matrixstack_pop(void);
mat4x4 * matrixstack_head(void);
void matrixstack_translate(float x, float y, float z);
void matrixstack_scale(float x, float y, float z);
void matrixstack_origin(void);
void matrixstack_rotate(float a, float rx, float ry, float rz);
void matrixstack_multiply(mat4x4 const* matrix);
