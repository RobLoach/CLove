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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../3rdparty/physfs/physfs.h"
#include "../3rdparty/SDL2/include/SDL.h"

void filesystem_init(char* argv0, int stats);
void filesystem_free();

const char* filesystem_getOS();
const char* filesystem_getSaveDirectory(const char* company, const char* projName);
int filesystem_read(char const* filename, char** output);
int filesystem_write(const char* name, const char* data);
int filesystem_append(const char* name, const char* data);
int filesystem_exists(const char* name);
int filesystem_equals(const char* a,const char* b,int l);
int filesystem_contain(const char* a, const char* b);
int filesystem_remove(const char* name);
int filesystem_isFile(const char* file, int mode);
bool filesystem_isDir(const char* dir);
bool filesystem_mkDir(const char* path);
const char* filesystem_getCurrentDirectory();
char** filesystem_enumerate(const char* path);
const char* filesystem_getUsrDir();
bool filesystem_setIdentity(const char* path);

bool filesystem_mount(const char* path, const char* mountPoint, int appendToPath);
bool filesystem_unmount(const char* path);

void filesystem_setSource(const char* source, const char* dir);
const char* filesystem_getSource();
