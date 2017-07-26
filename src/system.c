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
    int power = SDL_GetPowerInfo(&seconds, &percent);

	const char* state;
	if (power == SDL_POWERSTATE_UNKNOWN)
		state = "power_state_unknown";
	else if (power == SDL_POWERSTATE_CHARGED)
		state = "battery_charged";
	else if (power == SDL_POWERSTATE_CHARGING)
		state = "battery_charging";
	else if (power == SDL_POWERSTATE_ON_BATTERY)
		state = "not_plugged_running_on_battery";
	else if (power == SDL_POWERSTATE_NO_BATTERY)
		state = "plugged_in_no_battery";
	else
		state = "unknown";

   	system_PowerState powerState = {
        state,
        seconds,
        percent
    };
    return powerState;
}
