//
// simpleShape.cpp
//
// routines for adding triangles to create a new mesh
//
// Students are not to modify this file.
//

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <vector>
#include <iostream>

#include "simpleShape.h"

using namespace std;

///
// common variables...should probably make this a class and make these
// data members
///
vector<float> points;
vector<float> normals;
vector<float> uv;
float *pointArray = 0;
float *normalArray = 0;
float *uvArray = 0;

GLushort *elemArray = 0;

///
// clear the current shape
///
void clearShape(void)
{
	if (pointArray) {
		delete[] pointArray;
		pointArray = 0;
	}
	if (elemArray) {
		delete[] elemArray;
		elemArray = 0;
	}
	if (normalArray) {
		delete[] normalArray;
		normalArray = 0;
	}
	if (uvArray) {
		delete[] uvArray;
		uvArray = 0;
	}
	points.clear();
	normals.clear();
	uv.clear();
}

/**
* adds a triangle to the current shape using calculated normals
*/
void addTriangle(
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	// calculate the normal
	float ux = x1 - x0;
	float uy = y1 - y0;
	float uz = z1 - z0;

	float vx = x2 - x0;
	float vy = y2 - y0;
	float vz = z2 - z0;

	float nx = (uy * vz) - (uz * vy);
	float ny = (uz * vx) - (ux * vz);
	float nz = (ux * vy) - (uy * vx);

	// Attach the normal to all 3 vertices
	addTriangleWithNorms(
		x0, y0, z0, nx, ny, nz,
		x1, y1, z1, nx, ny, nz,
		x2, y2, z2, nx, ny, nz
		);
}

///
// adds a triangle to the current shape using calculated normals
///
void addTriangleWithUV(
	float x0, float y0, float z0, float u0, float v0,
	float x1, float y1, float z1, float u1, float v1,
	float x2, float y2, float z2, float u2, float v2)
{
	// calculate the normal
	float ux = x1 - x0;
	float uy = y1 - y0;
	float uz = z1 - z0;

	float vx = x2 - x0;
	float vy = y2 - y0;
	float vz = z2 - z0;

	float nx = (uy * vz) - (uz * vy);
	float ny = (uz * vx) - (ux * vz);
	float nz = (ux * vy) - (uy * vx);

	// Attach the normal to all 3 vertices
	addTriangleWithNorms(
		x0, y0, z0, nx, ny, nz,
		x1, y1, z1, nx, ny, nz,
		x2, y2, z2, nx, ny, nz
		);

	// Attach the texture coordinates
	uv.push_back(u0);
	uv.push_back(v0);
	uv.push_back(u1);
	uv.push_back(v1);
	uv.push_back(u2);
	uv.push_back(v2);
}

///
// adds a triangle to the current shape using supplied normals
///
void addTriangleWithNorms(
	float x0, float y0, float z0, float nx0, float ny0, float nz0,
	float x1, float y1, float z1, float nx1, float ny1, float nz1,
	float x2, float y2, float z2, float nx2, float ny2, float nz2)
{
	points.push_back(x0);
	points.push_back(y0);
	points.push_back(z0);
	points.push_back(1.0f);

	normals.push_back(nx0);
	normals.push_back(ny0);
	normals.push_back(nz0);

	points.push_back(x1);
	points.push_back(y1);
	points.push_back(z1);
	points.push_back(1.0f);

	normals.push_back(nx1);
	normals.push_back(ny1);
	normals.push_back(nz1);

	points.push_back(x2);
	points.push_back(y2);
	points.push_back(z2);
	points.push_back(1.0f);

	normals.push_back(nx2);
	normals.push_back(ny2);
	normals.push_back(nz2);

}
void addTriangleWithNormUV(
	float x0, float y0, float z0, float n00, float n01, float n02, float u0, float v0,
	float x1, float y1, float z1, float n10, float n11, float n12, float u1, float v1,
	float x2, float y2, float z2, float n20, float n21, float n22, float u2, float v2)
{
	
	// Attach the normal to all 3 vertices
	addTriangleWithNorms(
		x0, y0, z0, n00, n01, n02,
		x1, y1, z1, n10, n11, n12,
		x2, y2, z2, n20, n21, n22
		);

	// Attach the texture coordinates
	uv.push_back(u0);
	uv.push_back(v0);
	uv.push_back(u1);
	uv.push_back(v1);
	uv.push_back(u2);
	uv.push_back(v2);
}

///
// gets the vertex points for the current shape
///
float *getVertices(void)
{
	// delete the old point array if we have one
	if (pointArray) {
		delete[] pointArray;
	}

	int n = points.size();
	if (n == 0) {
		return(NULL);
	}

	// create and fill a new point array
	pointArray = new float[n];
	for (int i = 0; i < n; i++) {
		pointArray[i] = points[i];
	}

	return pointArray;
}

///
// gets the normals for the current shape
///
float *getNormals(void)
{
	// delete the old normal array if we have one
	if (normalArray) {
		delete[] normalArray;
	}

	int n = normals.size();
	if (n == 0) {
		return(NULL);
	}

	// create and fill a new normal array
	normalArray = new float[n];
	for (int i = 0; i < n; i++) {
		normalArray[i] = normals[i];
	}

	return normalArray;
}

///
// gets the texture coords for the current shape
///
float *getUV(void)
{
	// delete the old texture coordinate array if we have one
	if (uvArray) {
		delete[] uvArray;
	}

	int n = uv.size();
	if (n == 0) {
		return(NULL);
	}

	// create and fill a new texture coordinate array
	uvArray = new float[n];
	for (int i = 0; i < n; i++) {
		uvArray[i] = uv[i];
	}

	return uvArray;
}

///
// gets the  array of elements for the  current shape
///
GLushort *getElements(void)
{
	// delete the old element array if we have one
	if (elemArray) {
		delete[] elemArray;
	}

	int n = points.size();
	if (n == 0) {
		return(NULL);
	}

	// create and fill a new element array
	elemArray = new GLushort[n];
	for (int i = 0; i < n; i++) {
		elemArray[i] = i;
	}

	return elemArray;
}

///
// returns number of vertices in current shape
///
int nVertices(void)
{
	return points.size() / 4;
}
