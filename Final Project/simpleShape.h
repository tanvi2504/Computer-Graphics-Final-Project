//
// simpleShape.h
//
// Prototypes for routines for adding triangles to create a new mesh.
//
// Students are not to modify this file.
//

#ifndef _SIMPLESHAPE_H_
#define _SIMPLESHAPE_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

void clearShape(void);

void addTriangle(
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2);

void addTriangleWithUV(
	float x0, float y0, float z0, float u0, float v0,
	float x1, float y1, float z1, float u1, float v1,
	float x2, float y2, float z2, float u2, float v2);

void addTriangleWithNorms(
	float x0, float y0, float z0, float nx0, float ny0, float nz0,
	float x1, float y1, float z1, float nx1, float ny1, float nz1,
	float x2, float y2, float z2, float nx2, float ny2, float nz2);
void addTriangleWithNormUV(
	float x0, float y0, float z0, float n00, float n01, float n02, float u0, float v0,
	float x1, float y1, float z1, float n10, float n11, float n12, float u1, float v1,
	float x2, float y2, float z2, float n20, float n21, float n22, float u2, float v2);

GLushort *getElements(void);

float *getVertices(void);
float *getNormals(void);
float *getUV(void);

int nVertices(void);

#endif
