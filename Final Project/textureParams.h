//
//  textureParams.h
//
//  Created by Joe Geigel on 1/23/13.
//

#ifndef _TEXTUREPARAMS_H_
#define _TEXTUREPARAMS_H_

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

void loadTexture(void);
void setUpTexture(GLuint program);

#endif 
