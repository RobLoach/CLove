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

#include <stdlib.h>
#include <string.h>

static struct
{
	char* clipboardText;
} moduleData;

const char* system_getOS() {
    return SDL_GetPlatform();
}

int system_getProcessorCount()
{
	return SDL_GetCPUCount();
}

const char* system_getClipboardText()
{
	char* text = SDL_GetClipboardText(); //it's not null terminated

	if (text)
	{
		moduleData.clipboardText = malloc(strlen(text) + 1);
		strcpy(moduleData.clipboardText, text);
		SDL_free(text);

		return moduleData.clipboardText;
	}

	return "";
}

void system_setClipboardText(const char* text)
{
	moduleData.clipboardText = text;
	SDL_SetClipboardText(text);
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
