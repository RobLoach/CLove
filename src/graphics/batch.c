/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include <string.h>

#include "../tools/utils.h"

#include "batch.h"
#include "graphics.h"

static struct {
	GLuint sharedIndexBuffer;
	uint16_t *sharedIndexBufferData;
	int indexBufferSize;
	mat4x4 tr2d;
	mat3x3 transform;
} moduleData;


static void graphics_batch_makeIndexBuffer(int quadCount) {
	if(quadCount <= moduleData.indexBufferSize) {
		return;
	}

	// Round up to multiple of 128
	quadCount = (quadCount + 127) & ~127;

	moduleData.sharedIndexBufferData = realloc(moduleData.sharedIndexBufferData, quadCount * 6 * sizeof(uint16_t));
	for(int i = moduleData.indexBufferSize; i < quadCount; ++i) {
		moduleData.sharedIndexBufferData[6*i] = 4*i;
		moduleData.sharedIndexBufferData[6*i+1] = 4*i+1;
		moduleData.sharedIndexBufferData[6*i+2] = 4*i+2;
		moduleData.sharedIndexBufferData[6*i+3] = 4*i+2;
		moduleData.sharedIndexBufferData[6*i+4] = 4*i+1;
		moduleData.sharedIndexBufferData[6*i+5] = 4*i+3;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.sharedIndexBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) *6*quadCount, moduleData.sharedIndexBufferData, GL_STATIC_DRAW);
	moduleData.indexBufferSize = quadCount;
}

void graphics_batch_init(void) {
	glGenBuffers(1, &moduleData.sharedIndexBuffer);
	moduleData.sharedIndexBufferData = NULL;
	moduleData.indexBufferSize = 0;
	graphics_batch_makeIndexBuffer(128);
}

void graphics_Batch_changeBufferSize(graphics_Batch* batch, int newSize) {
	SAFE_FREE(batch->vertexData);
	batch->vertexData = malloc(4 * newSize * sizeof(graphics_Vertex));
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferData(GL_ARRAY_BUFFER, 4*newSize*sizeof(graphics_Vertex), batch->vertexData, batch->usage);
	batch->maxCount = newSize;
	batch->insertPos = 0;

	graphics_batch_makeIndexBuffer(newSize);
}


void graphics_Batch_new(graphics_Batch* batch, graphics_Image const* texture, int maxSize, graphics_BatchUsage usage) {

	batch->texture = texture;
	batch->vertexData = malloc(4*maxSize * sizeof(graphics_Vertex));
	glGenBuffers(1, &batch->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferData(GL_ARRAY_BUFFER, 4*maxSize*sizeof(graphics_Vertex), batch->vertexData, usage);
	batch->maxCount = maxSize;
	batch->insertPos = 0;

	graphics_batch_makeIndexBuffer(maxSize);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(2*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(4*sizeof(float)));

	batch->bound = false;
	batch->usage = usage;
	batch->colorSet = false;
	batch->colorUsed = false;
	batch->color.x = 1.0f;
	batch->color.y = 1.0f;
	batch->color.z = 1.0f;
	batch->color.w = 1.0f;
}

static const vec2 batchQuadPts[4] = {
	{0.0f,0.0f},{0.0f,1.0f},{1.0f,0.0f},{1.0f,1.0f}
};

int graphics_Batch_add(graphics_Batch* batch, graphics_Quad const* q, float x, float y, float r, float sx, float sy, float ox, float oy, float kx, float ky) {
	if(batch->insertPos == batch->maxCount) {
		return -1;
	}

	m3x3_newTransform2d(&moduleData.transform, x, y, r, sx, sy, ox, oy, kx, ky,
			q->w * batch->texture->width, q->h * batch->texture->height);

	graphics_Vertex *v = batch->vertexData+ 4*batch->insertPos;

	for(int i = 0; i < 4; ++i) {
		m3x3_mulV2(&v[i].pos, &moduleData.transform, batchQuadPts+i);
		v[i].color = batch->color;
	}

	batch->colorUsed |= batch->colorSet;

	v[0].uv.x = q->x;
	v[0].uv.y = q->y;
	v[1].uv.x = q->x;
	v[1].uv.y = q->y + q->h;
	v[2].uv.x = q->x + q->w;
	v[2].uv.y = q->y;
	v[3].uv.x = q->x + q->w;
	v[3].uv.y = q->y + q->h;

	if(batch->bound) {
	} else {
        //glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
		//glBufferSubData(GL_ARRAY_BUFFER, batch->insertPos * 4, 4*sizeof(graphics_Vertex), v);
	}

	return batch->insertPos++;
}

static const graphics_Quad fullQuad = {
	0.0f,0.0f,1.0f,1.0f
};

static float const defaultColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

void graphics_Batch_draw(graphics_Batch const* batch,
		float x, float y, float r, float sx, float sy,
		float ox, float oy, float kx, float ky) {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, batch->texture->texID);

	glBufferData(GL_ARRAY_BUFFER, 4*batch->maxCount*sizeof(graphics_Vertex), batch->vertexData, batch->usage);

	m4x4_newTransform2d(&moduleData.tr2d, x, y, r, sx, sy, ox, oy, kx, ky);
	float const * color = batch->colorUsed ? defaultColor : graphics_getColor();

	graphics_drawArray(&fullQuad, &moduleData.tr2d, moduleData.sharedIndexBuffer, batch->insertPos*6, GL_TRIANGLES,
			GL_UNSIGNED_SHORT,  color, 1.0f, 1.0f);

}

void graphics_Batch_bind(graphics_Batch *batch) {
	batch->bound = true;
}

void graphics_Batch_clear(graphics_Batch *batch) {
	batch->insertPos = 0;
	batch->colorUsed = false;
}

void graphics_Batch_flush(graphics_Batch *batch) {
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4*batch->insertPos*sizeof(graphics_Vertex), batch->vertexData);
}

void graphics_Batch_unbind(graphics_Batch *batch) {
	if(!batch->bound) {
		return;
	}

	graphics_Batch_flush(batch);

	batch->bound = false;
}

void graphics_Batch_setColor(graphics_Batch *batch, float r, float g, float b, float a) {
	batch->color.x = r;
	batch->color.y = g;
	batch->color.z = b;
	batch->color.w = a;
	batch->colorSet = true;
}

void graphics_Batch_clearColor(graphics_Batch *batch) {
	batch->color.x = 1.0f;
	batch->color.y = 1.0f;
	batch->color.z = 1.0f;
	batch->color.w = 1.0f;
	batch->colorSet = false;
}

void graphics_Batch_free(graphics_Batch* batch) {
	glDeleteBuffers(1, &batch->vbo);
	glDeleteBuffers(1, &moduleData.sharedIndexBuffer);

	SAFE_FREE(batch->vertexData);
}


