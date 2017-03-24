/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"
#include "../3rdparty/SDL2/include/SDL.h"
#include "../tools/utils.h"

#ifdef CLOVE_WINDOWS
#include <direct.h>
#define getcwd _getcwd // apparently getcwd is dreprecated on windows
#include <io.h>
#define access _access
#endif
#ifdef CLOVE_UNIX
#include <unistd.h>
#endif

static struct {
    const char* os;
} moduleData;

void filesystem_init(char* argv0, int stats) {
#ifdef CLOVE_MACOSX
    moduleData.os = "osx";
#elif CLOVE_LINUX
    moduleData.os = "linux";
#elif CLOVE_WINDOWS
    moduleData.os = "windows";
#elif CLOVE_ANDROID
	moduleData.is = "android";
#else
    moduleData.os = "This OS is not supported";
#endif
}

const char* filesystem_getOS() {
    return moduleData.os;
}

const char* filesystem_getSaveDirectory(const char* company, const char* projName) {
    return SDL_GetPrefPath(company, projName);
}

int filesystem_contain(const char* a, const char* b) {
    if(strstr(a,b))
        return 1;
    else
        return 0;
}

int filesystem_compare(const char* a, const char* b, int l) {
    if(strncmp(a, b, l) == 0)
        return 1;
    else
        return 0;
}

int filesystem_exists(const char* name)
{
    FILE* file = fopen(name,"r");
    if(!file){
        return 0;
    }

    fclose(file);
    return 1;
}

int filesystem_write(const char* name, const char* data)
{
    FILE* file = fopen(name, "w");
    if(!file){
        printf("%s No file named %s",name,"%s creating one");
        return -1;
    }

    fseek(file,0,SEEK_END);
    long size = ftell(file);
    rewind(file);

    fprintf(file, data);
    fclose(file);

    return size;
}

int filesystem_append(const char* name, const char* data) {
    FILE* file = fopen(name, "a");

    fseek(file,0,SEEK_END);
    long size = ftell(file);
    rewind(file);

    fprintf(file, data);
    fclose(file);

    return size;
}

const char* filesystem_getCurrentDirectory() {
    char buffer[1024];
    const char* dir = getcwd(buffer, sizeof(buffer));
    if (dir != NULL)
        return dir;

    printf("Error, Could not get the current directory \n");
    return NULL;
}

/**
 * mode can be:
 * 0, wether or not the file exist
 * 2, write only
 * 4, read only
 * 6, read and write
 * default is 0
 */
 int filesystem_isFile(const char* file, int mode) {

    if (access(file, mode) != -1)
        return 0;

    return -1;
}

int filesystem_read(char const* filename, char** output) {
    FILE* infile = fopen(filename, "r");
    if(!infile) {
        return -1;
    }

    fseek(infile, 0, SEEK_END);
    long size = ftell(infile);
    rewind(infile);

    *output = malloc(size+1);
    fread(*output, size, 1, infile);
    fclose(infile);
    (*output)[size] = 0;
    return size;
}


const char* filesystem_getSource() {
    return SDL_GetBasePath();
}


int filesystem_remove(const char* name) {
    remove(name);
    return 0;
}
