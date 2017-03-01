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

#include "shader.h"
#include "image.h"
#include "quad.h"
#include "vertex.h"
#include "graphics.h"

#include "../image/imagedata.h"
#include "../math/vector.h"

#ifdef EMSCRIPTEN
#include <GLES2/gl2.h>
#else
#include "../3rdparty/glew/include/GL/glew.h"
#endif

typedef enum {
  graphics_MeshDrawMode_fan       = GL_TRIANGLE_FAN,
  graphics_MeshDrawMode_strip     = GL_TRIANGLE_STRIP,
  graphics_MeshDrawMode_triangles = GL_TRIANGLES,
  graphics_MeshDrawMode_points    = GL_POINTS
} graphics_MeshDrawMode;

typedef struct {
    GLuint vbo;
    GLuint ibo;
    graphics_Image* image;
    graphics_MeshDrawMode drawMode;
    graphics_Vertex* vertices;
    int vertexCount;
    unsigned int* indices;
    int indexCount;
    mat4x4 tr2d;
    bool hasTexture;
    graphics_Shader plainColorShader;
} graphics_Mesh;

void graphics_Mesh_new(graphics_Mesh* mesh, int vertexCount, graphics_Vertex* vertices, int indexCount, unsigned int* indices, graphics_MeshDrawMode drawMode);
void graphics_Mesh_free(graphics_Mesh* mesh);

void graphics_Mesh_setTexture(graphics_Mesh* mesh, graphics_Image* image);

void graphics_Mesh_setVertices(graphics_Mesh* mesh, graphics_Vertex* vertices, int vertexCount);
void graphics_Mesh_setIndices(graphics_Mesh* mesh, unsigned int* indices, int indexCount);

void graphics_Mesh_draw(graphics_Mesh* mesh, float x, float y, float r, float sx, float sy, float ox, float oy, float kx, float ky);






