/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "obj_loader.h"

#include "../../tools/utils.h"

//TODO add IBO

static struct
{
    GLuint vbo;
    GLuint ibo;

    mat4x4 tr3d;

} moduleData;

int graphics_ObjLoader_new(obj_loader *loader, const char* path)
{


    vec3* temp_vertex = malloc(sizeof(vec3));
    vec2* temp_uv = malloc(sizeof(vec2));
    vec3* temp_normal = malloc(sizeof(vec3));

    uint32_t* vertexIndices = NULL;
    uint32_t* uvIndices = NULL;
    uint32_t* normalIndices = NULL;


    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        printf("%s %s \n", "Error opening object file: ", path);
        return -1;
    }


    int temp_vertex_index = 0;
    int temp_uv_index = 0;
    int temp_normal_index = 0;

    int index_vertices = 0;
    int index_uv = 0;
    int index_normal = 0;
    while(1)
    {
        //TODO what if is larger than 256?
        char lineHeader[256];
        int res = fscanf(file,"%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0)
        {
            vec3 vertex;

            int res = fscanf(file,"%f %f %f \n", &vertex.x, &vertex.y, &vertex.z);

            temp_vertex = realloc(temp_vertex, sizeof(vec3) * (temp_vertex_index + 1));

            temp_vertex[temp_vertex_index].x = vertex.x;
            temp_vertex[temp_vertex_index].y = vertex.y;
            temp_vertex[temp_vertex_index].z = vertex.z;
            temp_vertex_index++;
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            vec2 uv;

            int res = fscanf(file, "%f %f \n", &uv.x, &uv.y);

            temp_uv = realloc(temp_uv, sizeof(vec2) * (temp_uv_index + 1));

            temp_uv[temp_uv_index].x = uv.x;
            temp_uv[temp_uv_index].y = uv.y;
            temp_uv_index++;
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            vec3 nor;

            int res = fscanf(file, "%f %f %f \n", &nor.x, &nor.y, &nor.z);

            temp_normal= realloc(temp_normal, sizeof(vec3) * (temp_normal_index + 1));

            temp_normal[temp_normal_index].x = nor.x;
            temp_normal[temp_normal_index].y = nor.y;
            temp_normal[temp_normal_index].z = nor.z;
            temp_normal_index++;
        }

        else if (strcmp(lineHeader,"f") == 0)
        {
            uint32_t vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",

                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                    &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                    &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

            if (matches != 9){
                printf("%s %s \n", "Error, there must be 9 parameters to parameter face(f) inside: ", path);
                return -2;
            }

            vertexIndices = realloc(vertexIndices, sizeof(uint32_t) * (index_vertices + 4));

            vertexIndices[index_vertices] = vertexIndex[0];
            index_vertices += 1;
            vertexIndices[index_vertices] = vertexIndex[1];
            index_vertices += 1;
            vertexIndices[index_vertices] = vertexIndex[2];
            index_vertices +=1;

            uvIndices = realloc(uvIndices, sizeof(uint32_t) * (index_uv + 4));

            uvIndices[index_uv] = uvIndex[0];
            index_uv += 1;
            uvIndices[index_uv] = uvIndex[1];
            index_uv += 1;
            uvIndices[index_uv] = uvIndex[2];
            index_uv += 1;

            normalIndices = realloc(normalIndices, sizeof(uint32_t) * (index_normal + 4));

            normalIndices[index_normal] = normalIndex[0];
            index_normal++;
            normalIndices[index_normal] = normalIndex[1];
            index_normal++;
            normalIndices[index_normal] = normalIndex[2];
            index_normal++;
        }
    }

    //printf("%d %d %d \n",index_vertices, index_normal, index_uv);

    loader->vertex = malloc(sizeof(graphics_Vertex3d) * index_vertices + 1);
    for (int i = 0; i < index_vertices; i++)
    {
        uint32_t vertexIndex = vertexIndices[i];
        uint32_t uvIndex = uvIndices[i];
        uint32_t normalIndex = normalIndices[i];

        vec3 vertex = temp_vertex[vertexIndex-1];
        vec2 uv = temp_uv[uvIndex-1];
        vec3 normal = temp_normal[normalIndex-1];

        //printf("%f %f %f \n", vertex.x, vertex.y, vertex.z);
        //printf("%f %f \n", uv.x, uv.y);
        //printf("%f %f %f \n", normal.x, normal.y, normal.z);

        loader->vertex[i].pos = vertex;
        loader->vertex[i].uv = uv;
        loader->vertex[i].color.x = 1.0f;
        loader->vertex[i].color.y = 1.0f;
        loader->vertex[i].color.z = 1.0f;
        loader->vertex[i].color.w = 1.0f;
    }

    fclose(file);

    glGenBuffers(1, &moduleData.vbo);

    glBindBuffer(GL_ARRAY_BUFFER, moduleData.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(loader->vertex), loader->vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex3d), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex3d), (GLvoid const*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex3d), (GLvoid const*)(7*sizeof(float)));

    free(temp_normal);
    free(temp_uv);
    free(temp_vertex);

    free(normalIndices);
    free(uvIndices);
    free(vertexIndices);

    return 0;
}

static const graphics_Quad quad = {0.0f, 0.0f, 1.0f, 1.0f};

void graphics_ObjLoader_draw(obj_loader *loader, graphics_Image const* image, float x, float y, float z)
{

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(loader->vertex), loader->vertex);

    mat4x4 trans;
    m4x4_newIdentity(&trans);
    m4x4_newTranslation(&trans, x, y, z);

    mat4x4 scale;
    m4x4_newIdentity(&scale);
    m4x4_newScaling(&scale, 36, 36, 36);

    m4x4_mulM4x4(&moduleData.tr3d, &trans, &scale);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image->texID);

    graphics_drawArray3d(&quad, &moduleData.tr3d,  0, 12, GL_TRIANGLES, GL_UNSIGNED_INT,
            graphics_getColor(), image->width * quad.w, image->height * quad.h, image->width * quad.w);

}



















