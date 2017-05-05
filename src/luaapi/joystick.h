/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once 

#include "../3rdparty/lua/lua.h"
#include "../joystick.h"
#include "tools.h"

typedef struct {
    joystick_Joystick* joystick;
} l_joystick_Joystick;

void l_joystick_register(lua_State* state);

void l_joystick_pressed(int id, int button);
void l_joystick_released(int id, int button);
