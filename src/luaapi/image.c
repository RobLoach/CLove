/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../3rdparty/lua/lauxlib.h"

#include "image.h"
#include "tools.h"
#include "../image/imagedata.h"

static struct {
    int imageDataMT;
    image_ImageData* imageData;
} moduleData;

image_ImageData* l_image_getImageData() {
    return moduleData.imageData;
}

int l_image_newImageData(lua_State* state) {
    moduleData.imageData = (image_ImageData*)lua_newuserdata(state, sizeof(image_ImageData));
    int s1type = lua_type(state, 1);
    if(s1type == LUA_TSTRING) {
        moduleData.imageData->path = lua_tostring(state, 1);
        if(image_ImageData_new_with_filename(moduleData.imageData, lua_tostring(state, 1)) == 0){
            lua_pushstring(state, "Could not load image file: ");
            lua_pushstring(state, lua_tostring(state, 1));
            lua_pushstring(state, ", reason: ");
            lua_pushstring(state, image_error());
            lua_concat(state, 4);
            return lua_error(state);
        }
    } else if(s1type == LUA_TNUMBER && lua_type(state, 2) == LUA_TNUMBER) {
        image_ImageData_new_with_size(moduleData.imageData, lua_tointeger(state, 1), lua_tointeger(state, 2));
    } else {
        lua_pushstring(state, "need filename or size for imagedata");
        return lua_error(state);
    }

    lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.imageDataMT);
    lua_setmetatable(state, -2);

    return 1;
}

static int l_image_ImageData_getWidth(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    lua_pushinteger(state, image_ImageData_getWidth(imagedata));
    return 1;
}

static int l_image_ImageData_getHeight(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    lua_pushinteger(state, image_ImageData_getHeight(imagedata));
    return 1;
}

static int l_image_ImageData_getDimensions(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    lua_pushinteger(state, image_ImageData_getWidth(imagedata));
    lua_pushinteger(state, image_ImageData_getHeight(imagedata));
    return 2;
}

static int l_image_ImageData_getPath(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    lua_pushstring(state,imagedata->path);
    return 1;
}

/*
static int l_image_ImageData_getString(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    unsigned char* string = image_ImageData_getSurface(imagedata);
    lua_pushstring(state, string);
    return 1;
}
*/

// named save in imagedata.h and .c
static int l_image_ImageData_encode(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    const char* format = lua_tostring(state, 2);
    const char* filename = lua_tostring(state, 3);
    image_ImageData_save(imagedata, format, filename);
    return 1;
}

static int l_image_ImageData_setPixel(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    int x = lua_tointeger(state, 2);
    int y = lua_tointeger(state, 3);
    pixel p;
    p.r = lua_tointeger(state, 4);
    p.g = lua_tointeger(state, 5);
    p.b = lua_tointeger(state, 6);
    p.a = lua_tointeger(state, 7);

    image_ImageData_setPixel(imagedata,x,y,p);
    return 1;
}

static int l_image_ImageData_getPixel(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    int x = lua_tointeger(state, 2);
    int y = lua_tointeger(state, 3);
    int w = image_ImageData_getWidth(imagedata);

    unsigned char* surface = image_ImageData_getSurface(imagedata);

    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 0;

    //TODO check what happens when the image is RGB not RGBA
    int index = (y * image_ImageData_getWidth(imagedata) + x) * image_ImageData_getChannels(imagedata) - 1;

    int channels = image_ImageData_getChannels(imagedata);
    int return_no = 4;

    if (channels == 4)
    {
        red = surface[index] & 255;
        green = surface[++index] & 255;
        blue = surface[++index] & 255;
        alpha = surface[++index] & 255;

        lua_pushinteger(state, red);
        lua_pushinteger(state, green);
        lua_pushinteger(state, blue);
        lua_pushinteger(state, alpha);
        lua_pushinteger(state, image_ImageData_getPixel(imagedata,x,y));

        return_no = 4;
    }
    else if (channels == 3)
    {
        red = surface[index] & 255;
        green = surface[++index] & 255;
        blue = surface[++index] & 255;

        lua_pushinteger(state, red);
        lua_pushinteger(state, green);
        lua_pushinteger(state, blue);
        lua_pushinteger(state, image_ImageData_getPixel(imagedata,x,y));

        return_no = 3;
    }
    else if (channels == 2)
    {
        red = surface[index] & 255;
        green = surface[++index] & 255;

        lua_pushinteger(state, red);
        lua_pushinteger(state, green);
        lua_pushinteger(state, image_ImageData_getPixel(imagedata,x,y));

        return_no = 2;
    }
    else if (channels == 1)
    {
        red = surface[index] & 255;

        lua_pushinteger(state, red);
        lua_pushinteger(state, image_ImageData_getPixel(imagedata,x,y));
        return_no = 1;
    }

    return return_no;

}

static int l_image_ImageData_getChannels(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    lua_pushinteger(state, image_ImageData_getChannels(imagedata));
    return 1;
}

static int l_image_gcImageData(lua_State* state) {
    image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
    image_ImageData_free(imagedata);
    return 0;
}

static luaL_Reg const regFuncs[] = {
    {"newImageData", l_image_newImageData},
    {NULL, NULL}
};

l_checkTypeFn(l_image_isImageData, moduleData.imageDataMT)
l_toTypeFn(l_image_toImageData, image_ImageData)

static luaL_Reg const imageDataMetatableFuncs[] = {
    //{"getString", l_image_ImageData_getString},
    {"getWidth", l_image_ImageData_getWidth},
    {"getHeight", l_image_ImageData_getHeight},
    {"getDimensions", l_image_ImageData_getDimensions},
    {"getPixel", l_image_ImageData_getPixel},
    {"setPixel", l_image_ImageData_setPixel},
    {"getChannels", l_image_ImageData_getChannels},
    {"encode", l_image_ImageData_encode},
    {"getPath", l_image_ImageData_getPath},
    {"__gc", l_image_gcImageData},
    {NULL, NULL}
};

int l_image_register(lua_State* state) {
    l_tools_registerModule(state, "image", regFuncs);

    moduleData.imageDataMT = l_tools_makeTypeMetatable(state, imageDataMetatableFuncs);

    return 0;
}


