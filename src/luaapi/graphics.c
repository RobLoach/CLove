/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../3rdparty/lua/lauxlib.h"
#include "../3rdparty/lua/lua.h"
#include "../3rdparty/CMath/cmath.h"

#include "graphics.h"

#include "../graphics/graphics.h"
#include "../graphics/matrixstack.h"
#include "../graphics/shader.h"
#include "../graphics/gltools.h"

#include "tools.h"
#include "image.h"
#include "graphics_mesh.h"
#include "graphics_batch.h"
#include "graphics_image.h"
#include "graphics_quad.h"
#include "graphics_font.h"
#include "graphics_geometry.h"
#include "graphics_shader.h"
#include "graphics_window.h"

static int l_graphics_getBackgroundColor(lua_State* state) {
    float * colors = graphics_getBackgroundColor();

    for(int i = 0; i < 4; ++i) {
        lua_pushnumber(state, (int)(colors[i] * 255));
    }

    return 4;
}

static int l_graphics_getColor(lua_State* state) {
    float * colors = graphics_getColor();

    for(int i = 0; i < 4; ++i) {
        lua_pushnumber(state, (int)(colors[i] * 255));
    }

    return 4;
}

static int l_graphics_setBackgroundColor(lua_State* state) {
    int red   = lua_tointeger(state, 1);
    int green = lua_tointeger(state, 2);
    int blue  = lua_tointeger(state, 3);
    int alpha = luaL_optinteger(state, 4, 255);

    float scale = 1.0f / 255.0f;

    graphics_setBackgroundColor(red * scale, green * scale, blue * scale, alpha * scale);
    return 0;
}

static int l_graphics_setColor(lua_State* state) {
    int red   = lua_tointeger(state, 1);
    int green = lua_tointeger(state, 2);
    int blue  = lua_tointeger(state, 3);
    int alpha = luaL_optinteger(state, 4, 255);

    float scale = 1.0f / 255.0f;

    graphics_setColor(red * scale, green * scale, blue * scale, alpha * scale);
    return 0;
}

static int l_graphics_clear(lua_State* state) {
    graphics_clear();
    return 0;
}

static const graphics_Quad defaultQuad = {
    .x = 0.0f,
    .y = 0.0f,
    .w = 1.0f,
    .h = 1.0f
};

static int l_graphics_draw(lua_State* state) {
    l_graphics_Image const* image = NULL;
    l_graphics_Batch const* batch = NULL;
    l_graphics_Mesh const* mesh = NULL;

    graphics_Quad const * quad = &defaultQuad;
    int baseidx = 1;

    if (l_graphics_isImage(state, baseidx)) {
        image = l_graphics_toImage(state, baseidx);
        if(l_graphics_isQuad(state, baseidx + 1)) {
            quad = l_graphics_toQuad(state, baseidx + 1);
            baseidx += 1;
        }
    } else if (l_graphics_isBatch(state, baseidx)) {
        batch = l_graphics_toBatch(state, baseidx);
    } else if (l_graphics_isMesh(state, baseidx)) {
        mesh = l_graphics_toMesh(state, baseidx);
    } else {
        lua_pushstring(state, "expected image,spritebatch or mesh");
        lua_error(state);
    }

    float x  = luaL_optnumber(state, baseidx+1, 0.0f);
    float y  = luaL_optnumber(state, baseidx+2, 0.0f);
    float r  = luaL_optnumber(state, baseidx+3, 0.0f);
    float sx = luaL_optnumber(state, baseidx+4, 1.0f);
    float sy = luaL_optnumber(state, baseidx+5, 1.0f);
    float ox = luaL_optnumber(state, baseidx+6, 0.0f);
    float oy = luaL_optnumber(state, baseidx+7, 0.0f);
    float kx = luaL_optnumber(state, baseidx+8, 0.0f);
    float ky = luaL_optnumber(state, baseidx+9, 0.0f);

    if (image) {
        graphics_Image_draw(&image->image, quad, x, y, r, sx, sy, ox, oy, kx, ky);
    } else if (batch) {
        graphics_Batch_draw(&batch->batch, x, y, r, sx, sy, ox, oy, kx, ky);
    } else if (mesh) {
        graphics_Mesh_draw(&mesh->mesh, x, y, r, sx, sy, ox, oy, kx, ky);
    }
    return 0;
}

static int l_graphics_push(lua_State* state) {
    if(matrixstack_push()) {
        lua_pushstring(state, "Matrix stack overflow");
        return lua_error(state);
    }
    return 0;
}

static int l_graphics_pop(lua_State* state) {
    if(matrixstack_pop()) {
        lua_pushstring(state, "Matrix stack underrun");
        return lua_error(state);
    }
    return 0;
}

static int l_graphics_set_look_at(lua_State* state) {
    float px = l_tools_toNumberOrError(state, 1);
    float py = l_tools_toNumberOrError(state, 2);
    float pz = l_tools_toNumberOrError(state, 3);
    float tx = l_tools_toNumberOrError(state, 4);
    float ty = l_tools_toNumberOrError(state, 5);
    float tz = l_tools_toNumberOrError(state, 6);
    float ux = l_tools_toNumberOrError(state, 7);
    float uy = l_tools_toNumberOrError(state, 8);
    float uz = l_tools_toNumberOrError(state, 9);
    graphics_set_look_at(px,py,pz,tx,ty,tz,ux,uy,uz);
    return 0;
}

static int l_graphics_translate(lua_State* state) {
    float x = l_tools_toNumberOrError(state, 1);
    float y = l_tools_toNumberOrError(state, 2);
    float z = luaL_optinteger(state, 3, 1);

    matrixstack_translate(x, y, z);
    return 0;
}

static int l_graphics_scale(lua_State* state) {
    float x = l_tools_toNumberOrError(state, 1);
    float y = luaL_optnumber(state, 2, x);
    float z = luaL_optnumber(state, 3, 1);

    matrixstack_scale(x, y, z);
    return 0;
}

static int l_graphics_rotate(lua_State* state) {
    float top = lua_gettop(state);
    float a = 0;
    float x = 0;
    float y = 0;
    float z = 0;

    if(top == 1) {
        a = l_tools_toNumberOrError(state, 1);
        matrixstack_rotate(a);
    }else if(top == 4){ // one of X,Y or Z must be different than 0 !
        a = l_tools_toNumberOrError(state, 1);
        x = l_tools_toNumberOrError(state, 2);
        y = l_tools_toNumberOrError(state, 3);
        z = l_tools_toNumberOrError(state, 4);
        matrixstack_rotate_3d(a, x, y, z);
    }else {
        luaL_error(state,"love.graphics.rotate accepts 1 or 4 parms");
    }

    return 0;
}

static int l_graphics_setCamera(lua_State* state) {
    const char* type = l_tools_toStringOrError(state, 1);

    if (strncmp(type, "2d", 2) == 0){
        float left = luaL_optnumber(state, 2, 0);
        float right = luaL_optnumber(state, 3, graphics_getWidth());
        float bottom = luaL_optnumber(state, 4, graphics_getHeight());
        float top = luaL_optnumber(state, 5, 0);
        float zNear = luaL_optnumber(state, 6, 0.1f);
        float zFar = luaL_optnumber(state, 7, 100);
        graphics_set_camera_2d(left, right, bottom, top, zNear, zFar);
    }else if (strncmp(type, "3d", 2) == 0){
        float fov = luaL_optnumber(state, 2, 70);
        float ratio = luaL_optnumber(state, 3, graphics_getWidth() / graphics_getHeight());
        float zNear = luaL_optnumber(state, 4, 0.1f);
        float zFar = luaL_optnumber(state, 5, 100);
        graphics_set_camera_3d(fov, ratio, zNear, zFar);
    }else
        luaL_error(state, "No camera type named: ", type);

    return 0;
}

static int l_graphics_origin(lua_State* state) {
    matrixstack_origin();
    return 0;
}

static int l_graphics_shear(lua_State* state) {
    lua_pushstring(state, "not implemented");
    lua_error(state);
    return 0;
}

static int l_graphics_setColorMask(lua_State* state) {
    if(lua_isnone(state, 1)) {
        graphics_setColorMask(true, true, true, true);
        return 0;
    } else {
        for(int i = 2; i < 5; ++i) {
            if(lua_isnone(state, i)) {
                lua_pushstring(state, "illegal paramters");
                return lua_error(state);
            }
        }
    }

    bool r = lua_toboolean(state, 1);
    bool g = lua_toboolean(state, 2);
    bool b = lua_toboolean(state, 3);
    bool a = lua_toboolean(state, 4);

    graphics_setColorMask(r,g,b,a);

    return 0;
}

static int l_graphics_getColorMask(lua_State* state) {
    bool r,g,b,a;
    graphics_getColorMask(&r, &g, &b, &a);
    lua_pushboolean(state, r);
    lua_pushboolean(state, g);
    lua_pushboolean(state, b);
    lua_pushboolean(state, a);

    return 4;
}

static const l_tools_Enum l_graphics_BlendMode[] = {
    {"additive",       graphics_BlendMode_additive},
    {"alpha",          graphics_BlendMode_alpha},
    {"subtractive",    graphics_BlendMode_subtractive},
    {"multiplicative", graphics_BlendMode_multiplicative},
    {"premultiplied",  graphics_BlendMode_premultiplied},
    {"replace",        graphics_BlendMode_replace},
    {"screen",         graphics_BlendMode_screen},
    {NULL, 0}
};


static int l_graphics_setBlendMode(lua_State* state) {
    graphics_BlendMode mode = l_tools_toEnumOrError(state, 1, l_graphics_BlendMode);
    graphics_setBlendMode(mode);
    return 0;
}

static int l_graphics_getBlendMode(lua_State* state) {
    l_tools_pushEnum(state, graphics_getBlendMode(), l_graphics_BlendMode);
    return 1;
}

static int l_graphics_setScissor(lua_State* state) {
    if(lua_isnone(state, 1)) {
        graphics_clearScissor();
        return 0;
    } else {
        for(int i = 2; i < 5; ++i) {
            if(lua_isnone(state, i)) {
                lua_pushstring(state, "illegal paramters");
                return lua_error(state);
            }
        }
    }

    int x = l_tools_toNumberOrError(state, 1);
    int y = l_tools_toNumberOrError(state, 2);
    int w = l_tools_toNumberOrError(state, 3);
    int h = l_tools_toNumberOrError(state, 4);

    graphics_setScissor(x,y,w,h);

    return 0;
}

static int l_graphics_getScissor(lua_State* state) {
    int x,y,w,h;
    bool scissor = graphics_getScissor(&x, &y, &w, &h);
    if(!scissor) {
        return 0;
    }

    lua_pushinteger(state, x);
    lua_pushinteger(state, y);
    lua_pushinteger(state, w);
    lua_pushinteger(state, h);

    return 4;
}


static int l_graphics_getWidth(lua_State* state) {
    lua_pushnumber(state, graphics_getWidth());
    return 1;
}

static int l_graphics_getHeight(lua_State* state) {
    lua_pushnumber(state, graphics_getHeight());
    return 1;
}

static int l_graphics_reset(lua_State* state) {
    graphics_reset();
    return 0;
}


static luaL_Reg const regFuncs[] = {
    {"setBackgroundColor", l_graphics_setBackgroundColor},
    {"getBackgroundColor", l_graphics_getBackgroundColor},
    {"setColor",           l_graphics_setColor},
    {"getColor",           l_graphics_getColor},
    {"clear",              l_graphics_clear},
    {"draw",               l_graphics_draw},
    {"push",               l_graphics_push},
    {"pop",                l_graphics_pop},
    {"origin",             l_graphics_origin},
    {"setCamera",          l_graphics_setCamera},
    {"lookAt",             l_graphics_set_look_at},
    {"rotate",             l_graphics_rotate},
    {"scale",              l_graphics_scale},
    {"shear",              l_graphics_shear},
    {"translate",          l_graphics_translate},
    {"setColorMask",       l_graphics_setColorMask},
    {"getColorMask",       l_graphics_getColorMask},
    {"setBlendMode",       l_graphics_setBlendMode},
    {"getBlendMode",       l_graphics_getBlendMode},
    {"setScissor",         l_graphics_setScissor},
    {"getScissor",         l_graphics_getScissor},
    {"getWidth",           l_graphics_getWidth},
    {"getHeight",          l_graphics_getHeight},
    {"reset",              l_graphics_reset},
    {NULL, NULL}
};


int l_graphics_register(lua_State* state) {
    l_tools_registerModule(state, "graphics", regFuncs);

    l_graphics_shader_register(state);
    l_graphics_image_register(state);
    l_graphics_quad_register(state);
    l_graphics_font_register(state);
    l_graphics_batch_register(state);
    l_graphics_window_register(state);
    l_graphics_geometry_register(state);

    return 0;
}
