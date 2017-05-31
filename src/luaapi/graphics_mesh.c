/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_mesh.h"

static struct {
    int meshMT;
    void* vertices;
    int verticesSize;
    unsigned int* indices;
    int indicesSize;
}moduleData;

static const l_tools_Enum l_graphics_MeshDrawMode[] = {
    {"fan",       graphics_MeshDrawMode_fan},
    {"strip",     graphics_MeshDrawMode_strip},
    {"triangles", graphics_MeshDrawMode_triangles},
    {"points",    graphics_MeshDrawMode_points},
    {NULL, 0}
};

static void feedVertices(int size) {
    if (moduleData.verticesSize <  size) {
        free(moduleData.vertices);
        moduleData.vertices = malloc(size);
        moduleData.verticesSize = size;
    }
}

static void feedIndices(int size) {
    if (moduleData.indicesSize <  size) {
        free(moduleData.indices);
        moduleData.indices= malloc(size);
        moduleData.indicesSize = size;
    }
}

static void readVertex(lua_State* state, graphics_Vertex* vertices) {
    if (lua_objlen(state, -1) < 4)
       l_tools_trowError(state, "Table entry is not a valid vertex");

    float* t = (float*)vertices;

    for (int i = 0; i < 4; i++) {
        lua_rawgeti(state, -1, i+1);
        t[i] = l_tools_toNumberOrError(state, -1);
        lua_pop(state, 1);
    }

    //colors are optional
    for (int i = 4; i < 8; i++) {
        lua_rawgeti(state, -1, i+1);
        t[i] = luaL_optnumber(state, -1, 255.0f) / 255.0f;
        lua_pop(state, 1);
    }
}

static void readIndice(lua_State* state, int* indices, int count) {
    if (lua_objlen(state, -1) == 0)
        l_tools_trowError(state, "Invalid indices table");

    int* t = indices;

    for (int i = 0; i < count; i++) {
        lua_rawgeti(state, -1, i+1);
        t[i] = lua_tonumber(state, -1);
        lua_pop(state, 1);
    }

}

static int readIndices(lua_State* state, int indx, int numberOfIndex) {
    if (!lua_istable(state, indx)) {
        l_tools_trowError(state, "Need table of indices");
        return 0;
    }

    int count = lua_objlen(state, indx);
    for (int i = 0; i < count; i++) {
        lua_rawgeti(state, indx, i+1);
        readIndice(state, moduleData.indices + i, numberOfIndex);
        lua_pop(state, 1);
    }

    return count;
}

static int readVertices(lua_State* state, int indx) {
    if (!lua_istable(state, indx)) {
        lua_pushstring(state, "Need table of vertices");
        lua_error(state);
        return 0;
    }

    int count = lua_objlen(state, indx);
    feedVertices(count * sizeof(graphics_Vertex));

    for(int i = 0; i < count; ++i) {
        lua_rawgeti(state, indx, i+1);
        readVertex(state, ((graphics_Vertex*)moduleData.vertices) + i);
        lua_pop(state, 1);
    }

    return count;
}


int l_graphics_newMesh(lua_State* state) {

    int vertexCount = readVertices(state, 1);
    int numberOfIndex = l_tools_toNumberOrError(state, 2);

    feedIndices(numberOfIndex * sizeof(unsigned int));
    readIndices(state, 3, numberOfIndex);

    graphics_MeshDrawMode3d mode = l_tools_toEnumOrError(state, 4, l_graphics_MeshDrawMode);

    l_graphics_Mesh* mesh = lua_newuserdata(state, sizeof(l_graphics_Mesh));
    graphics_Mesh_new(&mesh->mesh, vertexCount, (graphics_Vertex*)moduleData.vertices, numberOfIndex, moduleData.indices, mode);

    //mesh->textureRef = luaL_ref(state, LUA_REGISTRYINDEX);

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.meshMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_graphics_Mesh_setVertices(lua_State* state) {
    l_graphics_Mesh* mesh = l_graphics_toMesh(state, 1);

    int vertexCount = readVertices(state, 2);
    graphics_Mesh_setVertices(&mesh->mesh, (graphics_Vertex*)moduleData.vertices, vertexCount);

    return 0;
}

static int l_graphics_Mesh_setIndices(lua_State* state) {
    l_graphics_Mesh* mesh = l_graphics_toMesh(state, 1);

    int numberOfIndex = l_tools_toNumberOrError(state, 2);
    readIndices(state, 3, numberOfIndex);
    graphics_Mesh_setIndices(&mesh->mesh, moduleData.indices, numberOfIndex);

    return 0;
}

static int l_graphics_Mesh_setTexture(lua_State* state) {
    l_graphics_Mesh* mesh = l_graphics_toMesh(state, 1);
    graphics_Image* image = l_graphics_toImage(state, 2);

    graphics_Mesh_setTexture(&mesh->mesh, image);
    luaL_unref(state, LUA_REGISTRYINDEX, mesh->textureRef);
    lua_settop(state, 2);
    mesh->textureRef = luaL_ref(state, LUA_REGISTRYINDEX);

    return 0;
}

static int l_graphics_gcMesh(lua_State* state) {
    l_graphics_Mesh* mesh = l_graphics_toMesh(state, 1);

    graphics_Mesh_free(&mesh->mesh);

    luaL_unref(state, LUA_REGISTRYINDEX, mesh->textureRef);

    return 0;
}

static luaL_Reg const meshMetatableFuncs[] = {
    {"setVertices",        l_graphics_Mesh_setVertices},
    {"setIndices",         l_graphics_Mesh_setIndices},
    {"setTexture",         l_graphics_Mesh_setTexture},
    {"__gc",               l_graphics_gcMesh},
    {NULL, NULL}
};

static luaL_Reg const meshFreeFuncs[] = {
    {"newMesh",           l_graphics_newMesh},
    {NULL, NULL}
};


void l_graphics_mesh_register(lua_State* state) {
    l_tools_registerFuncsInModule(state, "graphics", meshFreeFuncs);
    moduleData.meshMT = l_tools_makeTypeMetatable(state, meshMetatableFuncs);
}

l_checkTypeFn(l_graphics_isMesh, moduleData.meshMT)
l_toTypeFn(l_graphics_toMesh, l_graphics_Mesh)
