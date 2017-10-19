//
//  finalMain
//
//  Main program for Final Project
//
//  This code can be compiled as either C or C++.
//
//  @author:  Tanvi Raut(tmr5413@rit.edu)
//

#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#endif

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

/*
#include <pthread.h>
void junk(){
	int i;
	i = pthread_getconcurrency();
};
*/

#include "shaderSetup.h"
#include "simpleShape.h"
#include "shapes.h"
#include "viewParams.h"
#include "lightingParams.h"
#include "textureParams.h"

#ifdef __cplusplus
using namespace std;
#endif

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// We need seven vertex buffers and seven element buffers:
// three set for  (texture mapped), and four set
// for  (Phong shaded and gouraud shaded)
//
// Array layout:
// element 0:  eye1
// element 1:  eye2
// element 2:  neck
// element 3:  body
// element 4:  leg1
// element 5:  leg2
// element 6:  teapot
// 
//
GLuint vbuffer[7];
GLuint ebuffer[7];
int    numVerts[7];


// program IDs...for shader programs
GLuint pshader, tshader, tshader1, gshader;

//
// makeBuffer() - create a vertex or element array buffer
//
// @param target - which type of buffer to create
// @param data   - source of data for buffer (or NULL)
// @param size   - desired length of buffer
//
GLuint makeBuffer(GLenum target, const void *data, GLsizei size)
{
	GLuint buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, size, data, GL_STATIC_DRAW);

	return(buffer);
}

//
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
//
void createShape(int obj)
{
	// clear any previous shape
	clearShape();

	// make the shape
	makeShape(obj);

	// save the vertex count
	numVerts[obj] = nVertices();

	// get the points for the shape
	float *points = getVertices();
	int dataSize = numVerts[obj] * 4 * sizeof(float);

	// get the normals for the shape
	float *normals = getNormals();
	int ndataSize = numVerts[obj] * 3 * sizeof(float);

	// there may or may not be (u,v) information
	float *uv = getUV();
	int uvdataSize = 0;
	if (uv != NULL) {
		uvdataSize = numVerts[obj] * 2 * sizeof(float);
	}

	// get the element data for the shape
	GLushort *elements = getElements();
	int edataSize = numVerts[obj] * sizeof(GLushort);

	int totalSize = dataSize + ndataSize + uvdataSize;

	// generate the vertex buffer
	vbuffer[obj] = makeBuffer(GL_ARRAY_BUFFER, NULL, totalSize);
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, points);
	glBufferSubData(GL_ARRAY_BUFFER, dataSize, ndataSize, normals);
	if (uv != NULL) {
		glBufferSubData(GL_ARRAY_BUFFER, dataSize + ndataSize, uvdataSize, uv);
	}

	// generate the element buffer
	ebuffer[obj] = makeBuffer(GL_ELEMENT_ARRAY_BUFFER, elements, edataSize);
}

//
// selectBuffers() - bind the correct vertex and element buffers
//
// @param obj - which object to select
//
void selectBuffers(GLuint program, int obj) {

	// bind the buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer[obj]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer[obj]);

	// calculate the number of bytes of vertex and normal data
	int dataSize = numVerts[obj] * 4 * sizeof(float);
	int ndataSize = numVerts[obj] * 3 * sizeof(float);

	// set up the vertex attribute variables
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(dataSize));

	if (obj == OBJ_EYE1) {
		GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(dataSize + ndataSize));
	}
	if (obj == OBJ_EYE2) {
		GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(dataSize + ndataSize));
	}
	if (obj == OBJ_BODY) {
		GLuint vTexCoord1 = glGetAttribLocation(program, "vTexCoord1");
		glEnableVertexAttribArray(vTexCoord1);
		glVertexAttribPointer(vTexCoord1, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(dataSize + ndataSize));
	}
}


// Verify shader program creation
static void checkShaderError(GLuint program, const char *which)
{
	if (!program) {
#ifdef __cplusplus
		cerr << "Error setting " << which << " shader - "
			<< errorString(shaderErrorCode) << endl;
#else
		fprintf(stderr, "Error setting up %s shader - %s\n",
			which, errorString(shaderErrorCode));
#endif
		exit(1);
	}
}


// OpenGL initialization
void init()
{
	// Load texture image(s)
	loadTexture();

	// Load shaders, verifying each
	tshader = shaderSetup("texture.vert", "texture.frag");
	checkShaderError(tshader, "texture");

	tshader1 = shaderSetup("texture1.vert", "texture1.frag");
	checkShaderError(tshader, "texture1");

	pshader = shaderSetup("phong.vert", "phong.frag");
	checkShaderError(pshader, "phong");

	gshader = shaderSetup("gouraud.vert", "gouraud.frag");
	checkShaderError(gshader, "gouraud");

	// Create all our objects
	createShape(OBJ_EYE1);
	createShape(OBJ_EYE2);
	createShape(OBJ_NECK);
	createShape(OBJ_BODY);
	createShape(OBJ_LEG1);
	createShape(OBJ_LEG2);
	createShape(OBJ_BALL);

	// Some openGL initialization...probably best to keep
	glEnable(GL_DEPTH_TEST);
	 glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	//----
	glEnable(GL_TEXTURE_2D);
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
}


#ifdef __cplusplus
extern "C" {
#endif

	void display(void)
	{
		// clear and draw params..
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// first, the eye1
		glUseProgram(tshader);

		// set up viewing and projection parameters
		setUpFrustum(tshader);

		// set up the texture information
		setUpTexture(tshader);
		

		// set up the camera
		setUpCamera(tshader,
			3.2f, 3.0f, 6.5f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			);

		// set up transformations for the eye1
		setUpTransforms(tshader,
			0.27f, 0.27f, 0.27f,
			90.0, 0.0f, 0.0f,
			-0.24f, 1.60f, 0.0f
			);

		// draw it
		selectBuffers(tshader, OBJ_EYE1);
		glDrawElements(GL_TRIANGLES, numVerts[OBJ_EYE1],
			GL_UNSIGNED_SHORT, (void *)0);

		// now, draw the eye2

		glUseProgram(tshader);

		// set up viewing and projection parameters
		setUpFrustum(tshader);

		// set up the texture information
		setUpTexture(tshader);
		

		// set up the camera
		setUpCamera(tshader,
			3.2f, 3.0f, 6.5f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			);

		setUpTransforms(tshader,
			0.27f, 0.27f, 0.27f,
			90.0, 0.0f, 0.0f,
			0.24f, 1.60f, 0.0f);

		// draw it
		selectBuffers(tshader, OBJ_EYE2);
		glDrawElements(GL_TRIANGLES, numVerts[OBJ_EYE2],
			GL_UNSIGNED_SHORT, (void *)0);

		//-------------
		// now, the neck
		glUseProgram(pshader);

		// set up viewing and projection parameters
		setUpFrustum(pshader);

		//set up lighing and shading
		setUpPhong(pshader);

		// set up the camera
		setUpCamera(pshader,
			3.2f, 3.0f, 6.5f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			);

		// set up transformations for the neck
		setUpTransforms(pshader,
			0.25f, 0.25f, 0.25f,
			-11.5f, 0.0f, 0.0f,
			0.0f, 1.30f, 0.0f
			);

		// draw it
		selectBuffers(pshader, OBJ_NECK);
		glDrawElements(GL_TRIANGLES, numVerts[OBJ_NECK],
			GL_UNSIGNED_SHORT, (void *)0);

		// now, draw the body

		glUseProgram(tshader1);

		// set up viewing and projection parameters
		setUpFrustum(tshader1);

		//set up the shader
		setUpTexture(tshader1);

		// set up the camera
		setUpCamera(tshader1,
			3.2f, 3.0f, 6.5f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			);

		setUpTransforms(tshader1,
			0.51f, 0.51f, 0.51f,
			-15.0f, 0.0f, 90.0,
			0.0f, 0.5f, 0.0f);

		// draw it
		selectBuffers(tshader1, OBJ_BODY);
		glDrawElements(GL_TRIANGLES, numVerts[OBJ_BODY],
			GL_UNSIGNED_SHORT, (void *)0);

		///----------------------------------------------------------------------//
		glUseProgram(pshader);

		// set up viewing and projection parameters
		setUpFrustum(pshader);

		// set up the texture information
		setUpPhong(pshader);
		//setUpPhong(pshader);

		// set up the camera
		setUpCamera(pshader,
			3.2f, 3.0f, 6.5f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			);

		// set up transformations for the leg1
		setUpTransforms(pshader,
			0.12f, 0.12f, 0.12f,
			-15.0f, 0.0f, 90.0,
			-0.24f, -0.20f, 0.0f
			);

		// draw it
		selectBuffers(pshader, OBJ_LEG1);
		glDrawElements(GL_TRIANGLES, numVerts[OBJ_LEG1],
			GL_UNSIGNED_SHORT, (void *)0);


		glUseProgram(pshader);

		// set up viewing and projection parameters
		setUpFrustum(pshader);

		// set up the texture information
		setUpPhong(pshader);
		//setUpPhong(pshader);

		// set up the camera
		setUpCamera(pshader,
			3.2f, 3.0f, 6.5f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			);

		// set up transformations for the leg2
		setUpTransforms(pshader,
			0.12f, 0.12f, 0.12f,
			-15.0f, 0.0f, 90.0,
			0.24f, -0.20f, 0.0f
			);

		// draw it
		selectBuffers(pshader, OBJ_LEG2);
		glDrawElements(GL_TRIANGLES, numVerts[OBJ_LEG2],
			GL_UNSIGNED_SHORT, (void *)0);

	//----------------------------------------------		
		glUseProgram(gshader);

		// set up viewing and projection parameters
		setUpFrustum(gshader);

		// set up the texture information
		setUpGouraud(gshader);
		//setUpPhong(pshader);

		// set up the camera
		setUpCamera(gshader,
			3.2f, 3.0f, 6.5f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
			);

		// set up transformations for the teapot
		setUpTransforms(gshader,
			0.92f, 0.92f, 0.92f,
			0.0f, 0.0f, 0.0f,
			-1.74f, -0.30f, 0.0f
			);

		// draw it
		selectBuffers(gshader, OBJ_BALL);
		glDrawElements(GL_TRIANGLES, numVerts[OBJ_BALL],
			GL_UNSIGNED_SHORT, (void *)0);

		
		// swap the buffers
		glutSwapBuffers();
	}


	void keyboard(unsigned char key, int x, int y)
	{
		switch (key) {

		

		case 033:   // terminate the program
		case 'q': case 'Q':
			exit(0);
			break;
		}

		glutPostRedisplay();
	}

	

#ifdef __cplusplus
}
#endif

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("CG - Shading Assignment");

#ifndef __APPLE__
	glewInit();
#endif

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(animate);

	glutMainLoop();
	return 0;
}
