/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "mesh3d.h"

#include "../../tools/utils.h"

//TODO indices based on array size. eg: uint8,uint16 etc

void graphics_Mesh3d_new(graphics_Mesh3d* mesh, int vertexCount, graphics_Vertex* vertices, int indexCount, unsigned int* indices, graphics_MeshDrawMode3d drawMode) {

    mesh->hasTexture = false;

    graphics_Shader_new3d(&mesh->plainColorShader, NULL,
            "vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_cords )           {\n"
            "  return color;\n"
            "}\n");

    mesh->vertices = 0;
    mesh->indices = 0;

    mesh->drawMode = drawMode;

    mesh->vertexCount = 0;
    mesh->indexCount = 0;

    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ibo);

    graphics_Mesh3d_setVertices(mesh, vertices, vertexCount);
    graphics_Mesh3d_setIndices(mesh, indices, indexCount);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(2*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(4*sizeof(float)));
}

void graphics_Mesh3d_free(graphics_Mesh3d* mesh) {
    glDeleteBuffers(1, &mesh->ibo);
    glDeleteBuffers(1, &mesh->vbo);

    SAFE_FREE(mesh->indices);
    SAFE_FREE(mesh->vertices);
}

void graphics_Mesh3d_setTexture(graphics_Mesh3d* mesh, graphics_Image* image) {
    if (image != NULL) {
        mesh->hasTexture = true;
        mesh->image = image;
    } else
        mesh->hasTexture = false;
}

void graphics_Mesh3d_setVertices(graphics_Mesh3d* mesh, graphics_Vertex* vertices, int vertexCount) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(graphics_Vertex), vertices, GL_DYNAMIC_DRAW);

    if (vertexCount != mesh->vertexCount) {
        SAFE_FREE(mesh->vertices);
        mesh->vertices = malloc(sizeof(graphics_Vertex) * vertexCount);
        mesh->vertexCount = vertexCount;
    }

    memcpy(mesh->vertices, vertices, vertexCount * sizeof(graphics_Vertex));
}

void graphics_Mesh3d_setIndices(graphics_Mesh3d* mesh, unsigned int* indices, int indexCount) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);

    if (indexCount != mesh->indexCount) {
        SAFE_FREE(mesh->indices);
        mesh->indices = malloc(sizeof(unsigned int) * indexCount);
        mesh->indexCount = indexCount;
    }
    memcpy(mesh->indices, indices, sizeof(unsigned int) * indexCount);
}

static const graphics_Quad quad = {0.0f, 0.0f, 1.0f, 1.0f};
void graphics_Mesh3d_draw(graphics_Mesh3d* mesh, float x, float y, float z, float r, float sx, float sy, float ox, float oy, float kx, float ky) {

    graphics_Shader* shader = graphics_getShader();
    if (mesh->hasTexture) {
        /*
         * Set the default shader in case you previously
         * did not have a texture put
         */
        graphics_setShader(shader);
        //glEnable(GL_TEXTURE_2D); Deprecated in GLES 2
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->image->texID);
        } else
            graphics_setShader(&mesh->plainColorShader);

    glBufferData(GL_ARRAY_BUFFER, sizeof(graphics_Vertex) * mesh->vertexCount, mesh->vertices, GL_DYNAMIC_DRAW);


    //TODO make this one!
    //m4x4_newTransform2d(&mesh->tr3d, x, y, r, sx , sy, ox, oy, kx, ky);
    graphics_drawArray(&quad, &mesh->tr3d, mesh->ibo, mesh->indexCount, mesh->drawMode, GL_UNSIGNED_INT, graphics_getColor(), 1.0f, 1.0f);

    if (!mesh->hasTexture)
        graphics_setShader(shader);
}



