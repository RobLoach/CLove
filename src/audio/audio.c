/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "audio.h"
#include "../3rdparty/openal/include/AL/al.h"
#include "../3rdparty/openal/include/AL/alc.h"
#include <stdio.h>

static struct {
	ALCdevice* device;
	ALCcontext* context;
} moduleData;

void audio_init(int stats) {
	moduleData.device = alcOpenDevice(NULL);
	moduleData.context = alcCreateContext(moduleData.device, NULL);
	if(!alcMakeContextCurrent(moduleData.context)) {
		printf("%s \n", "Failed to initialite audio context");
	}
	audio_StreamInit();
	if (stats > 0)
		printf("%s %s \n ", "Debug: OpenAL version: ", alGetString(AL_VERSION));
}

void audio_setVolume(double value){
	alListenerf(AL_GAIN, value);
}

void audio_close () {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(moduleData.context);
	alcCloseDevice(moduleData.device);
}
