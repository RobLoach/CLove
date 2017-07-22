/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

typedef struct {
 	const char* state;
    int seconds;
    int percent;
} system_PowerState;

const char* system_getOS();
system_PowerState system_getPowerInfo();
