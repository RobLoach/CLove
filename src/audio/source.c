/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "source.h"

#include <stdio.h>

void audio_SourceCommon_init(audio_SourceCommon *source) {

	ALenum err; // openAL error checker

	alGenSources(1, &source->source);
	err = alGetError();

	if (err != AL_NO_ERROR)
		printf("Error: Could not generate openAL source \n");

	source->state = audio_SourceState_stopped;

	alSourcef(source->source, AL_PITCH, 1);
	err = alGetError();

	if (err != AL_NO_ERROR)
		printf("Error: Could not set openAL pitch \n");

	alSourcef(source->source, AL_GAIN, 1);
	err = alGetError();

	if (err != AL_NO_ERROR)
		printf("Error: Could not set openAL gain \n");

	alSource3f(source->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	err = alGetError();

	if (err != AL_NO_ERROR)
		printf("Error: Could not set openAL position \n");

	alSource3f(source->source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	err = alGetError();

	if (err != AL_NO_ERROR)
		printf("Error: Could not set openAL velocity \n");

	alSourcei(source->source, AL_LOOPING, AL_FALSE);
	err = alGetError();

	if (err != AL_NO_ERROR)
		printf("Error: Could not set openAL looping \n");

}

void audio_SourceCommon_setLooping(audio_SourceCommon const* source, int value){
  if (value == 0)
    alSourcei(source->source, AL_LOOPING, AL_FALSE);
  else
    alSourcei(source->source, AL_LOOPING, AL_TRUE);
}

void audio_SourceCommon_play(audio_SourceCommon *source) {
  source->state = audio_SourceState_playing;
  alSourcePlay(source->source);
}

void audio_SourceCommon_stop(audio_SourceCommon *source) {
  source->state = audio_SourceState_stopped;
  alSourceStop(source->source);
}

void audio_SourceCommon_free(audio_SourceCommon *source) {
  audio_SourceCommon_stop(source);
  alDeleteSources(1, &source->source);
}

audio_SourceState audio_SourceCommon_getState(audio_SourceCommon const *source) {
  ALint state;
  alGetSourcei(source->source, AL_SOURCE_STATE, &state);
  return state;
}

void audio_SourceCommon_pause(audio_SourceCommon *source) {
  if(source->state == audio_SourceState_playing) {
    alSourcePause(source->source);
    source->state = audio_SourceState_paused;
  }
}

void audio_SourceCommon_resume(audio_SourceCommon *source) {
  if(source->state == audio_SourceState_paused) {
    alSourcePlay(source->source);
    source->state = audio_SourceState_playing;
  }
}

bool audio_SourceCommon_isPlaying(audio_SourceCommon const* source) {
  return source->state == audio_SourceState_playing;
}

bool audio_SourceCommon_isStopped(audio_SourceCommon const* source) {
  return source->state == audio_SourceState_stopped;
}


bool audio_SourceCommon_isPaused(audio_SourceCommon const* source) {
  return source->state == audio_SourceState_paused;
}

void audio_SourceCommon_setVolume(audio_SourceCommon const* source, float gain) {
  alSourcef(source->source, AL_GAIN, gain);
}

float audio_SourceCommon_getVolume(audio_SourceCommon const* source) {
  float gain;
  alGetSourcef(source->source, AL_GAIN, &gain);
  return gain;
}

float audio_SourceCommon_getPitch(audio_SourceCommon const* source) {
  float gain;
  alGetSourcef(source->source, AL_PITCH, &gain);
  return gain;
}

void audio_SourceCommon_setPitch(audio_SourceCommon const* source, float gain) {
  alSourcef(source->source, AL_PITCH, gain);
}

void audio_SourceCommon_setVelocity(audio_SourceCommon const* source, float x, float y, float z){
  alSource3f(source->source, AL_VELOCITY, x, y, z);
}

void audio_SourceCommon_setPosition(audio_SourceCommon const* source, float x, float y, float z){
  //In order to have this working you need to have a mono sound file!
  //alSourcei(source->source, AL_SOURCE_RELATIVE, AL_FALSE);
  //alListener3f(AL_POSITION, x, y, z);
  //float vec[6] = {x, y, z, 0.0f, 1.0f, 0.0f};
  //alListenerfv(AL_ORIENTATION, vec);
  alSource3f(source->source, AL_POSITION, x, y, z);
}
