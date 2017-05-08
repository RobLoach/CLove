/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "utils.h"

char* util_concatenate(const char* first, const char* second)
{
    char* buffer = malloc(sizeof(strlen(first)+strlen(second)+1));
    strcpy(buffer, first);
    strcat(buffer, second);
    return buffer;
}
