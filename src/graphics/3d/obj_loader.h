/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../shader.h"
#include "../image.h"
#include "../quad.h"
#include "../vertex.h"
#include "../graphics.h"

#include "../../image/imagedata.h"
#include "../../math/vector.h"
#include "../../tools/gl.h"

/*
 * Note:
 * v: vertex
 * vt: texture coordinate of one vertex
 * vn: normal
 * f: face
 */

typedef struct
{
    const char* path;
    graphics_Vertex3d* vertex;
} obj_loader;

int graphics_ObjLoader_new(obj_loader* loader, const char* path);
void graphics_ObjLoader_draw(obj_loader* loader, const graphics_Image *image, float x, float y, float z);
