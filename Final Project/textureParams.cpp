//
//  textureParams
//
//  Created by Joe Geigel on 1/23/13.
//
//  Contributor:  Tanvi Raut
//
//  This code can be compiled as either C or C++.
//
//  20155 version
//

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
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

#include <SOIL.h>
#include<stdio.h>

#include "textureParams.h"

#ifdef __cplusplus
using namespace std;
#endif

// Add any global definitions and/or variables you need here.

///
// This function loads texture data to the GPU.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
///
void loadTexture(void)
{
	// Add your code here.
	/*
	** load an image file directly as a new OpenGL texture
	*/
	GLuint smiley = SOIL_load_OGL_texture(
		"eye1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_TEXTURE_REPEATS
		);
	/* check for an error during the load process */
	if (smiley == 0) {
		printf("SOIL loading error: '%s'\n",
			SOIL_last_result());
	}
	
	GLuint frowny = SOIL_load_OGL_texture(
		"wallEArm.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_TEXTURE_REPEATS
		);
	/* check for an error during the load process */
	
	if (frowny == 0) {
		printf("SOIL loading error: '%s'\n",
			SOIL_last_result());
	}
	
	//glDisable(GL_CULL_FACE);
	//binding the texture to sampler before using
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, smiley);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frowny);
}

///
// This function sets up the parameters for texture use.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
//
///

void setUpTexture(GLuint program)
{
	
	GLint front;
	GLint back;
	// Add your code here.



	//send the data to the fragment shader.
	glEnable(GL_TEXTURE_2D);

	front = glGetUniformLocation(program, "smiley");

	glUniform1i(front, 0);
	back = glGetUniformLocation(program, "frowny");

	glUniform1i(back, 1);

	//glActiveTexture(GL_TEXTURE0);
	

}
