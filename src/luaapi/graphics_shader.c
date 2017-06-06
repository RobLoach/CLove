/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../3rdparty/lua/lauxlib.h"

#include <string.h>
#include <stdlib.h>

#include "../3rdparty/slre/slre.h"
#include "../graphics/shader.h"
#include "../filesystem/filesystem.h"
#include "../math/minmax.h"

#include "tools.h"
#include "graphics_shader.h"
#include "graphics_image.h"

static void pushShaderInfoLog(lua_State *state, graphics_Shader const* shader);

static struct {
  int shaderMT;
  int currentShaderRef;
  struct slre fragmentSingleShaderDetectRegex;
  struct slre vertexShaderDetectRegex;
  void * sendValueBuffer;
  int sendValueBufferSize;
} moduleData;

static char const * fragmentSingleShaderDetectRegexSrc = "vec4\\s*effect\\s*\\(";
static char const * vertexShaderDetectRegexSrc = "vec4\\s*position\\s*\\(";


bool static isVertexShader(char const* str) {
  return slre_match(&moduleData.vertexShaderDetectRegex, str, strlen(str), NULL);
}

bool static isSingleFragmentShader(char const* str) {
  return slre_match(&moduleData.fragmentSingleShaderDetectRegex, str, strlen(str), NULL);
}

int static l_graphics_newShader(lua_State* state) {
  char const* vertexSrc = l_tools_toStringOrError(state, 1);
  char const* fragmentSrc = NULL;
  char * loadedFile1 = NULL;
  char * loadedFile2 = NULL;

  if(lua_isstring(state, 2)) {
    fragmentSrc = lua_tostring(state, 2);
    
    if(!isVertexShader(vertexSrc)) {
      // TODO
      int loadedFile1Size = filesystem_read(vertexSrc, &loadedFile1);
      (void) loadedFile1Size;
      if(!loadedFile1 || !isVertexShader(loadedFile1)) {
        free(loadedFile1);
        lua_pushstring(state, "input 1 is not a valid vertex shader");
        return lua_error(state);
      }
      vertexSrc = loadedFile1;
    }

    if(!isSingleFragmentShader(fragmentSrc)) {
      // TODO
      int loadedFile2Size = filesystem_read(fragmentSrc, &loadedFile2);
      (void)loadedFile2Size;

      if(!loadedFile2 || !isSingleFragmentShader(loadedFile2)) {
        free(loadedFile1);
        free(loadedFile2);
        lua_pushstring(state, "input 2 is not a valid fragment shader");
        return lua_error(state);
      }
      fragmentSrc = loadedFile2;
    }

  } else {
    if(isVertexShader(vertexSrc)) {
      // nothing required
    } else if(isSingleFragmentShader(vertexSrc)) {
      fragmentSrc = vertexSrc;
      vertexSrc = NULL;
    } else {
      // TODO
      int loadedFile1Size = filesystem_read(vertexSrc, &loadedFile1);
      (void) loadedFile1Size;
      if(!loadedFile1) {
        lua_pushstring(state, "could not open file");
        return lua_error(state);
      }

      if(isSingleFragmentShader(loadedFile1)) {
        fragmentSrc = loadedFile1;
        vertexSrc = NULL;
      } else if(isVertexShader(loadedFile1)) {
        vertexSrc = loadedFile1;
        fragmentSrc = NULL;
      } else {
        free(loadedFile1);
        lua_pushstring(state, "input is not a valid shader");
        return lua_error(state);
      }
    }
  }

  l_graphics_Shader * shader = lua_newuserdata(state, sizeof(l_graphics_Shader));
  graphics_Shader_new(&shader->shader, vertexSrc, fragmentSrc);
  
  lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.shaderMT);
  lua_setmetatable(state, -2);

  free(loadedFile1);
  free(loadedFile2);

  int const textureUnits = shader->shader.textureUnitCount;
  shader->referencedTextures = malloc(textureUnits * sizeof(int));
  for(int i = 0; i < textureUnits; ++i) {
    shader->referencedTextures[i] = LUA_NOREF;
  }

  return 1;
}

l_checkTypeFn(l_graphics_isShader, moduleData.shaderMT)
l_toTypeFn(l_graphics_toShader, l_graphics_Shader)

static int l_graphics_gcShader(lua_State* state) {
  l_graphics_Shader *shader = l_graphics_toShader(state, 1);
  free(shader->referencedTextures);

  // Unref textures to allow gc'ing them
  for(int i = 0; i < shader->shader.textureUnitCount; ++i) {
    luaL_unref(state, LUA_REGISTRYINDEX, shader->referencedTextures[i]);
  }
  
  graphics_Shader_free(&shader->shader);
  return 0;
}

static int l_graphics_getShader(lua_State *state) {
  lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.currentShaderRef);
  return 1;
}

static int l_graphics_setShader(lua_State *state) {
  if(lua_isnoneornil(state, 1)) {
    graphics_setDefaultShader();
    luaL_unref(state, LUA_REGISTRYINDEX, moduleData.currentShaderRef);
    moduleData.currentShaderRef = LUA_NOREF;
    return 0;
  } else if(l_graphics_isShader(state, 1)) {
    l_graphics_Shader* shader = l_graphics_toShader(state, 1);
    luaL_unref(state, LUA_REGISTRYINDEX, moduleData.currentShaderRef);
    lua_settop(state, 1);
    moduleData.currentShaderRef = luaL_ref(state, LUA_REGISTRYINDEX);
    graphics_setShader(&shader->shader);
    return 0;
  } else {
    lua_pushstring(state, "expected nil or shader");
    return lua_error(state);
  }
}

static void growBuffer(int size) {
  if(size > moduleData.sendValueBufferSize) {
    free(moduleData.sendValueBuffer);
    moduleData.sendValueBuffer = malloc(size);
    moduleData.sendValueBufferSize = size;
  }
}

#define mkScalarSendFunc(name, type, totypefunc) \
  static void name(lua_State *state, l_graphics_Shader* shader, graphics_ShaderUniformInfo const* info) { \
    int count = min(lua_gettop(state) - 2, info->elements);         \
                                                                    \
    growBuffer(sizeof(type) * count);                               \
    type * numbers = (type*)moduleData.sendValueBuffer;             \
                                                                    \
    for(int i = 0; i < count; ++i) {                                \
      numbers[i] = totypefunc(state, 3 + i);                        \
    }                                                               \
                                                                    \
    graphics_Shader_ ## name(&shader->shader, info, count, numbers);\
  }

mkScalarSendFunc(sendIntegers, GLint,   l_tools_toNumberOrError)
mkScalarSendFunc(sendFloats,   GLfloat, l_tools_toNumberOrError)
mkScalarSendFunc(sendBooleans, GLint,   l_tools_toBooleanOrError)

#undef mkScalarSendFunc

#define mkVectorSendFunc(name, valuetype, totypefunc) \
  static void name(lua_State *state, l_graphics_Shader *shader, graphics_ShaderUniformInfo const* info) { \
    int components = graphics_shader_toLoveComponents(info->type); \
    int count = min(lua_gettop(state) - 2, info->elements);         \
                                                                    \
    growBuffer(sizeof(valuetype) * count * components);             \
    valuetype* numbers = (valuetype*)moduleData.sendValueBuffer;    \
                                                                    \
    for(int i = 0; i < count; ++i) {                                \
      for(int j = 0; j < components; ++j) {                         \
        lua_rawgeti(state, i + 3, j + 1);                           \
        numbers[i*components+j] = totypefunc(state, -1);            \
      }                                                             \
    }                                                               \
                                                                    \
    graphics_Shader_ ## name(&shader->shader, info, count, numbers);\
  }

mkVectorSendFunc(sendIntegerVectors, GLint,   l_tools_toNumberOrError)
mkVectorSendFunc(sendFloatVectors,   GLfloat, l_tools_toNumberOrError)
mkVectorSendFunc(sendBooleanVectors, GLint,   l_tools_toBooleanOrError)


static void sendFloatMatrices(lua_State *state, l_graphics_Shader* shader, graphics_ShaderUniformInfo const* info) {
  int components = graphics_shader_toLoveComponents(info->type);
  int count = min(lua_gettop(state) - 2, info->elements);
  growBuffer(sizeof(float) * components * components * count);

  float* numbers = (float*)moduleData.sendValueBuffer;

  for(int i = 0; i < count; ++i) {
    for(int j = 0; j < components; ++j) {
      // Get j-th inner table (== matrix column) of i-th outer table (== matrix)
      lua_rawgeti(state, i + 3, j + 1);
      for(int k = 0; k < components; ++k) {
        // Get k-th entry (== matrix row) of j-th inner table (== matrix column)
        lua_rawgeti(state, -1, k + 1);
        numbers[i*components*components+j*components+k] = l_tools_toNumberOrError(state, -1);
        lua_pop(state, 1);
      }
      lua_settop(state, count + 2);
    }
  }

  //graphics_Shader_sendFloatMatrices(&shader->shader, info, count, numbers);
}

static void referenceAndSendTexture(lua_State *state, l_graphics_Shader* shader, graphics_ShaderUniformInfo const* info, GLuint texture) {
  int index = (graphics_ShaderTextureUnitInfo*) info->extra - shader->shader.textureUnits;
  lua_settop(state, 3);
  shader->referencedTextures[index] = luaL_ref(state, LUA_REGISTRYINDEX);
  //graphics_Shader_sendTexture(&shader->shader, info, texture);
}

static void sendSamplers(lua_State *state, l_graphics_Shader* shader, graphics_ShaderUniformInfo const* info) {
   if(l_graphics_isImage(state,3)) {
    l_graphics_Image * image = l_graphics_toImage(state, 3);
    referenceAndSendTexture(state, shader, info, image->image.texID);
  } else {
    lua_pushstring(state, "Expected texture");
    lua_error(state);
  }
}

static int l_graphics_Shader_send(lua_State *state) {
  l_assertType(state, 1, l_graphics_isShader); 
  l_graphics_Shader* shader = l_graphics_toShader(state, 1);

  char const* name = l_tools_toStringOrError(state, 2);

  return 0;
}

static int l_graphics_Shader_getExternVariable(lua_State* state) {
  l_assertType(state, 1, l_graphics_isShader); 
  l_graphics_Shader const* shader = l_graphics_toShader(state, 1);

  char const* name = l_tools_toStringOrError(state, 2);
  return 3;

errout:
  lua_pushnil(state);
  lua_pushnil(state);
  lua_pushnil(state);
  return 3;
}

static char const vertexName[] = "vertex shader:\n";
static char const fragmentName[] = "\npixel shader:\n";
static char const programName[] = "\nprogram:\n";

static void pushShaderInfoLog(lua_State *state, graphics_Shader const* shader) {
  int len = strlen(shader->warnings.fragment)
            + strlen(shader->warnings.vertex)
            + strlen(shader->warnings.program)
            + strlen(vertexName)
            + strlen(fragmentName)
            + strlen(programName);

  char * fullLog = malloc(len + 1);
  strcpy(fullLog, vertexName);
  strcat(fullLog, shader->warnings.vertex);
  strcat(fullLog, fragmentName);
  strcat(fullLog, shader->warnings.fragment);
  strcat(fullLog, programName);
  strcat(fullLog, shader->warnings.program);
 
  lua_pushstring(state, fullLog);

  free(fullLog);
}

static int l_graphics_Shader_getWarnings(lua_State *state) {
  l_assertType(state, 1, l_graphics_isShader); 
  l_graphics_Shader const* shader = l_graphics_toShader(state, 1);

  pushShaderInfoLog(state, &shader->shader);

  return 1;
}


static luaL_Reg const shaderMetatableFuncs[] = {
  {"__gc",              l_graphics_gcShader},
  {"send",              l_graphics_Shader_send},
  {"getExternVariable", l_graphics_Shader_getExternVariable},
  {"getWarnings",       l_graphics_Shader_getWarnings},
  {NULL, NULL}
};

static luaL_Reg const shaderFreeFuncs[] = {
  {"newShader", l_graphics_newShader},
  {"setShader", l_graphics_setShader},
  {"getShader", l_graphics_getShader},
  {NULL, NULL}
};

void l_graphics_shader_register(lua_State *state) {
  moduleData.currentShaderRef = LUA_NOREF;
  l_tools_registerFuncsInModule(state, "graphics", shaderFreeFuncs);
  moduleData.shaderMT  = l_tools_makeTypeMetatable(state, shaderMetatableFuncs);

  slre_compile(&moduleData.fragmentSingleShaderDetectRegex, fragmentSingleShaderDetectRegexSrc);
  slre_compile(&moduleData.vertexShaderDetectRegex, vertexShaderDetectRegexSrc);
}

