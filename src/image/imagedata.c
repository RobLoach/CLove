/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../3rdparty/stb/stb_image.c"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../3rdparty/stb/stb_image_write.h"
#include "imagedata.h"


int image_ImageData_new_with_filename(image_ImageData *dst, char const* filename) {
  int n;
  dst->surface = stbi_load(filename, &dst->w, &dst->h, &n, STBI_rgb_alpha);
  dst->c = STBI_rgb_alpha;
  dst->path = filename;
  
  if(!dst->surface){  //image could not be loaded
     printf("%s %s \n", "Error: No image data in file ", filename);
 	  return 0;
  }
  dst->pixels = (pixel*) dst->surface;
  return 1;
}

void image_ImageData_new_with_size(image_ImageData *dst, int width, int height) {
  dst->surface = malloc(sizeof(unsigned char) * width * height * 4);
  dst->w = width;
  dst->h = height;
  dst->c = 4; //RGBA ftw ^_^
  dst->path = "this is an image data.";
  memset(dst->surface, 0, sizeof(unsigned char) * width * height * 4);
  dst->pixels = (pixel*) dst->surface;
}

// return rgba(4) 
int image_ImageData_getChannels(image_ImageData *dst) {
	return dst->c;
}

int image_ImageData_getPixel(image_ImageData *dst, int x, int y) {
	return dst->surface[y * dst->w + x];
}

int image_ImageData_setPixel(image_ImageData *dst, int x, int y, pixel p) {
	pixel *pixels = (pixel*) dst->surface;
	pixels[y * dst->w + x] = p;
	
	dst->pixels = (pixel*) dst->surface;
	dst->pixels[y * dst->w + x] = p;
	return 1;
}

int image_ImageData_save(image_ImageData *dst, const char* format, const char* filename) {
	int succeded = 0;
	if (strncmp(format, "png", 3) == 0)
		succeded = stbi_write_png(filename, dst->w, dst->h, dst->c, (const float*)dst->pixels, 0);
	else if (strncmp(format, "bmp", 3) == 0)
     	succeded = stbi_write_bmp(filename, dst->w, dst->h, dst->c, (const float*)dst->surface);		
	else if (strncmp(format, "tga", 3) == 0)
     	succeded = stbi_write_tga(filename, dst->w, dst->h, dst->c, (const float*)dst->pixels);
	else if (strncmp(format, "hdr", 3) == 0)
     	succeded = stbi_write_hdr(filename, dst->w, dst->h, dst->c, (const float*)dst->surface);
	else 
		printf("%s %s %s \n", "Error, format:", format,  " is not avalabile.Only png,bmp,tga and hdr image formats are possible");
	if(succeded != 0)
		return 1;
	else {
		printf("%s %s \n", "Error: failed to save imageData: ", filename);
		return 0;
	}
}

unsigned char* image_ImageData_getSurface(image_ImageData *dst) {
	return dst->surface;
}

int image_ImageData_getWidth(image_ImageData *dst) {
	return dst->w;
}

int image_ImageData_getHeight(image_ImageData *dst) {
	return dst->h;
}

const char* image_error(void){
  return stbi_failure_reason();
}

void image_ImageData_free(image_ImageData *data) {
  free(data->surface);
}

void image_init() {
}
