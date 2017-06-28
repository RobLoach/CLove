/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "streamsource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// loaders
#include "wav_decoder.h"
#include "vorbis_decoder.h"

static struct {
    audio_StreamSource ** playingStreams;
    int playingStreamSize;
    int playingStreamCount;
    ALuint unqueueBuffer[6];
} moduleData;

static const char* get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot+1;
}

void audio_StreamInit() {
    moduleData.playingStreamCount = 0;
    moduleData.playingStreamSize  = 16;
    moduleData.playingStreams     = malloc(sizeof(audio_StreamSource*) * 16);
}

int audio_loadStream(audio_StreamSource *source, char const * filename) {
    int err = 1;

    source->decoderData = malloc(sizeof(audio_vorbis_DecoderData));

    //NOTE: CLove only supports vorbis files when it comes to streaming!
    //if (strncmp(get_filename_ext(filename), "wav", 3) == 0)
    //err = -1;
    //else
    err = audio_vorbis_loadStream(source->decoderData, filename);
    if (err == 0)
        err = 1;

    alGenSources(1, &source->source);
    alSourcef( source->source, AL_PITCH,    1.0f);
    alSourcef( source->source, AL_GAIN,     1.0f);
    alSource3f(source->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source->source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei( source->source, AL_LOOPING,  AL_FALSE);
    alGenBuffers(6, source->buffers);
    source->channels   = audio_vorbis_getChannelCount(source->decoderData);
    source->sampleRate = audio_vorbis_getSampleRate(source->decoderData);

    for(int i = 0; i < 6; ++i) {
        audio_vorbis_preloadStreamSamples(source->decoderData, 100);
        audio_vorbis_uploadSreamSamples(source->decoderData, source->buffers[i]);
    }
    source->loop = false;
    return err;
}


void audio_updateStreams() {
    for(int i = 0; i < moduleData.playingStreamCount; ++i) {
        audio_StreamSource* source = moduleData.playingStreams[i];

        ALuint src = source->source;
        ALint count;
        ALint queued;
        ALint state;
        alGetSourcei(src, AL_BUFFERS_PROCESSED, &count);
        alGetSourcei(src, AL_BUFFERS_QUEUED, &queued);
        alGetSourcei(src, AL_SOURCE_STATE, &state);
    

        int loaded = audio_vorbis_preloadStreamSamples(source->decoderData, 8000);
        if (count > 0 && source->state == audio_SourceState_playing) {

            if (source->state == audio_SourceState_stopped) {
                count = 0;
                loaded = 0;
                alSourceStop(src);
                moduleData.playingStreamCount--;
            }


            //printf("%d buffers free, %d queued, state=%d\n", count, queued, state);
            if(loaded == 0) {
                //moduleData.playingStreamCount = 0;
                if (source->loop)
                    audio_vorbis_rewindStream(source->decoderData);
                else
                    source->state = audio_SourceState_stopped;
            }

            //            for(int j = 0; j < count; ++j) {
            ALuint buf;
            alSourceUnqueueBuffers(src, 1, &buf);
            int uploaded = audio_vorbis_uploadSreamSamples(source->decoderData, buf);
            if (uploaded > 0 && queued < 7)
                alSourceQueueBuffers(src, 1, &buf);
            //          }
        }
    }
}

void audio_StreamSource_play(audio_StreamSource *source) {
    source->state = audio_SourceState_playing;
    alSourceQueueBuffers(source->source, 6, source->buffers);

    if(moduleData.playingStreamCount == moduleData.playingStreamSize) {
        moduleData.playingStreamSize *= 2;
        moduleData.playingStreams = realloc(moduleData.playingStreams, moduleData.playingStreamSize*sizeof(audio_StreamSource*));
    }

    moduleData.playingStreams[moduleData.playingStreamCount] = source;
    ++moduleData.playingStreamCount;

    alSourcePlay(source->source);
}

void audio_StreamSource_setLooping(audio_StreamSource* source, int value) {
    source->loop = value;
}

void audio_StreamSource_stop(audio_StreamSource* source) {
    source->state = audio_SourceState_stopped;
    if (source->state == audio_SourceState_playing)
        alSourceStop(source->source);
}

void audio_StreamSource_resume(audio_StreamSource* source) {
    if(source->state == audio_SourceState_paused)
        alSourcePlay(source->source);
    source->state = audio_SourceState_playing;
}

void audio_StreamSource_pause(audio_StreamSource* source) {
    if (source->state == audio_SourceState_playing)
        alSourcePause(source->source);
    source->state = audio_SourceState_paused;
}

void audio_StreamSource_setVolume(audio_StreamSource* source, float gain) {
    alSourcef(source->source, AL_GAIN, gain);
}

float audio_StreamSource_getVolume(audio_StreamSource* source) {
    float gain;
    alGetSourcef(source->source, AL_GAIN, &gain);
    return gain;
}

float audio_StreamSource_getPitch(audio_StreamSource* source) {
    float gain;
    alGetSourcef(source->source, AL_PITCH, &gain);
    return gain;
}

void audio_StreamSource_setPitch(audio_StreamSource* source, float gain) {
    alSourcef(source->source, AL_PITCH, gain);
}

void audio_StreamSource_setVelocity(audio_StreamSource* source, float x, float y, float z){
    alSource3f(source->source, AL_VELOCITY, x, y, z);
}

bool audio_StreamSource_isPlaying(audio_StreamSource* source) {
    return source->state == audio_SourceState_playing;
}

bool audio_StreamSource_isStopped(audio_StreamSource* source) {
    return source->state == audio_SourceState_stopped;
}

bool audio_StreamSource_isPaused(audio_StreamSource* source) {
    return source->state == audio_SourceState_paused;
}

void audio_StreamSource_setPosition(audio_StreamSource *source, float x, float y, float z) {
    alSource3f(source->source, AL_POSITION, x, y, z);
}

void audio_StreamSource_free(audio_StreamSource* source) {
    alDeleteSources(1, &source->source);
    alDeleteBuffers(6, source->buffers);
}

