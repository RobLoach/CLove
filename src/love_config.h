/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdbool.h>

#pragma once

typedef struct {
	const char* identity;
	int width;
	int height;
	const char* title;
	const char* version;
	bool vsync;
	bool resizable;
	/* true = window is visible
	 * flase = window is hidded
	 * Note: use setMode to make it visible
	 */
	bool window;
	bool bordless;
	/* stats is used for displaying debug info */
	bool stats;
	int minwidth, minheight;
	int x, y;
} love_WindowConfig;

typedef struct {
	love_WindowConfig window;
} love_Config;
