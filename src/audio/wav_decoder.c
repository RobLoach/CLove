/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "wav_decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcmp
#include <stdint.h> // for int16_t and int32_t

struct wavfile
{
  char    id[4];          // should always contain "RIFF"
  int32_t totallength;    // total file length minus 8
  char    wavefmt[8];     // should be "WAVEfmt "
  int32_t format;         // 16 for PCM format
  int16_t pcm;            // 1 for PCM format
  int16_t channels;       // channels
  int32_t frequency;      // sampling frequency
  int32_t bytes_per_second;
  int16_t bytes_by_capture;
  int16_t bits_per_sample;
  char    data[4];        // should always contain "data"
  int32_t bytes_in_data;
} wav;

int audio_wav_load(unsigned int buffer, char const * filename) {
	FILE* file = fopen(filename, "rb");

	if(file == NULL)
	{
		fprintf(stderr,"Can't read input file %s\n", filename);
		return 0;
	}

	audio_wav_DecoderData* data = malloc(sizeof(audio_wav_DecoderData));
	fseek(file,0,SEEK_END);
	data->size = ftell(file);
	fseek(file,0,SEEK_SET);

	data->readBuffer = malloc(data->size - 44);
	struct wavfile header;

	if ( fread(&header,sizeof(header),1,file) < 1 ) {
		fprintf(stderr,"Can't read input file header %s\n", filename);
		return 0;
	}

	fseek(file, 44, SEEK_SET);
	fread(data->readBuffer,1,data->size - 44, file);
	fseek(file,0,SEEK_SET);

	fseek(file, 24, SEEK_SET);
	fread(&data->samplerate, 1, 4, file);
	fseek(file, 0, SEEK_SET);

	int format;
	if (header.format == 8){
		switch(header.channels){
			case 1:
				format = AL_FORMAT_MONO8;
				break;
			case 2:
				format = AL_FORMAT_STEREO8;
				break;
		}
	}
	if (header.format == 16) {
		switch(header.channels){
			case 1:
				format = AL_FORMAT_MONO16;
				break;
			case 2:
				format = AL_FORMAT_STEREO16;
				break;
		}
	}
	if (header.format == 18) {
		switch(header.channels){
			case 1:
				format = AL_FORMAT_MONO8;
				break;
			case 2:
				format = AL_FORMAT_MONO16;
				break;
		}
	}


	alBufferData(buffer, format,  data->readBuffer, header.totallength, data->samplerate);
	free(data);

	return 1;
}
