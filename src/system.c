/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "system.h"

#include "3rdparty/SDL2/include/SDL.h"

const char* system_getOS() {
    return SDL_GetPlatform();
}

system_PowerState system_getPowerInfo() {
    int seconds, percent;
    int state = SDL_GetPowerInfo(&seconds, &percent);
    system_PowerState powerState = {
        state,
        seconds,
        percent
    };
    return powerState;
}
