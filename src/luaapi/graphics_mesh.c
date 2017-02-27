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
}moduleData;

static const l_tools_Enum l_graphics_MeshDrawMode[] = {
    {"fan",       graphics_MeshDrawMode_fan},
    {"strip",     graphics_MeshDrawMode_strip},
    {"triangles", graphics_MeshDrawMode_triangles},
    {"points",    graphics_MeshDrawMode_points},
    {NULL, 0}
};

int l_graphics_newMesh(lua_State* state) {

    int ref = luaL_ref(state, LUA_REGISTRYINDEX);   
    graphics_Mesh* mesh = (graphics_Mesh*)lua_newuserdata(state, sizeof(l_graphics_Mesh));
    //graphics_Image* image = l_graphics_toTextureOrError(state, 5);
    //graphics_MeshDrawMode mode = l_tools_toEnumOrEror(state, 6, l_graphics_MeshDrawMode);

    //size_t vertexCount = l_tools_toNumberOrError(state, 1);
    //TODO getVertices at 2 

    //int indexCount = l_tools_toNumberOrError(state, 4);

    //TODO getIndices at 5

   // graphics_Mesh_new(mesh, vertexCount, vertices, indexCount, indices, image, mode);

    return 1;
}

static int l_graphics_gcMesh(lua_State* state) {
    l_graphics_Mesh* mesh = l_graphics_toMesh(state, 1);

    graphics_Mesh_free(&mesh->mesh);

    luaL_unref(state, LUA_REGISTRYINDEX, mesh->meshDataRef);

    return 0;
}

static luaL_Reg const meshMetatableFuncs[] = {
    {"__gc",               l_graphics_gcMesh},
    {NULL, NULL}
};

static luaL_Reg const meshFreeFuncs[] = {
    {"newMesh",           l_graphics_newMesh},
    {NULL, NULL}
};


void l_graphics_Mesh_register(lua_State* state) {
    l_tools_registerFuncsInModule(state, "graphics", meshFreeFuncs);
    moduleData.meshMT = l_tools_makeTypeMetatable(state, meshMetatableFuncs);
}

    l_checkTypeFn(l_graphics_isMesh, moduleData.meshMT)
l_toTypeFn(l_graphics_toMesh, l_graphics_Mesh)
