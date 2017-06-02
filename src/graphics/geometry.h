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

#include "graphics.h"

typedef enum {
  graphics_DrawMode_fill,
  graphics_DrawMode_line
}graphics_DrawMode;

void graphics_geometry_init();

/*
 * Summary: Draws a rectangle
 * filled:
 * true -> the rectangles will be filled with color
 * false -> the rectangle will not be filled with color
 * x, y: position on X,Y axis
 * rotation: from -360 .. 360,default 0
 * sx, sy: scale, default 1
 * ox, oy: origin, default 0
 */
void graphics_geometry_rectangle(bool filled, float x, float y,
                                     float w, float h,
                                     float rotation,
                                     float sx, float sy,
                                     float ox, float oy);

/*
 * Summary: Draw a circle filled with color
 * x, y: position on X,Y axis
 * radius: size of the circle (from center)
 * segments: how many vertices should this circle have, default 12.
 */
void graphics_geometry_fillCircle(float x, float y, float radius, int segments, float r, float sx, float sy, float ox, float oy);
void graphics_geometry_lineCircle(float x, float y, float radius, int segments, float r, float sx, float sy, float ox, float oy);

/*
 * Summary: Draw vertices filled or not filled with color
 * x, y: position
 * vertices: position of the vertices to be drawn
 * count: the number of vertices to be draw, put #vertices
 */
void graphics_geometry_vertex(bool filled, float x, float y, int vertices[], int count);
/*
 * Summary: Draw a pixel at X, Y
 */
void graphics_geometry_points(float x, float y);

void graphics_geometry_free();

void graphics_geometry_setLineWidth(float width);
float graphics_geometry_getLineWidth();
