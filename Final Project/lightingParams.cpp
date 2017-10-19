//
//  lightingParams
//
//  Created by Joe Geigel on 1/23/13.
//
//  Contributor:  Tanvi Raut
//
//  This code can be compiled as either C or C++.
//
//  20155 version
//
#include <GL/glew.h>
#include <GL/glut.h>
#include "lightingParams.h"

// Add any global definitions and/or variables you need here.

///
// This function sets up the lighting, material, and shading parameters
// for the Phong shader.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
//
///

void setUpPhong(GLuint program)
{
	// Add your code here.
	//pass the parameters to the vertex shader.
	GLint amc = glGetUniformLocation(program, "ambient_material_color");
	GLint dmc = glGetUniformLocation(program, "diffuse_material_color");
	GLint smc = glGetUniformLocation(program, "specular_material_color");
	glUniform4f(amc, 0.5f, 0.1f, 0.9f, 1.0f);
	glUniform4f(dmc, 0.89f, 0.89f, 0.0f, 1.0f);
	glUniform4f(smc, 1.0f, 1.0f, 1.0f, 1.0f);
	GLint arc = glGetUniformLocation(program, "ambient_reflection_coef");
	GLint drc = glGetUniformLocation(program, "diffuse_reflection_coef");
	GLint src = glGetUniformLocation(program, "specular_reflection_coef");
	GLint se = glGetUniformLocation(program, "specular_exponent");
	glUniform1f(arc, 0.5f);
	glUniform1f(drc, 0.7f);
	glUniform1f(src, 5.0f);
	glUniform1f(se, 10.0f);
	GLint lsc = glGetUniformLocation(program, "light_source_color");
	GLint lsp = glGetUniformLocation(program, "light_source_position");
	glUniform4f(lsc, 1.0f, 1.0f, 0.0f, 1.0f);
	glUniform4f(lsp, 0.0f, 5.0f, 2.0f, 1.0f);
	GLint alc = glGetUniformLocation(program, "ambientLightColor");
	glUniform4f(alc, 0.5f, 0.5f, 0.5f, 1.0f);




}

///
// This function sets up the lighting, material, and shading parameters
// for the Gouraud shader.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
//
///
void setUpGouraud(GLuint program)
{
	// Add your code here.
	//pass the parameters to the vertex shader.
	GLint amc = glGetUniformLocation(program, "ambient_material_color");
	GLint dmc = glGetUniformLocation(program, "diffuse_material_color");
	GLint smc = glGetUniformLocation(program, "specular_material_color");
	glUniform4f(amc, 0.5f, 0.1f, 0.9f, 1.0f);
	glUniform4f(dmc, 0.49f, 0.0f, 0.0f, 1.0f);
	glUniform4f(smc, 1.0f, 1.0f, 1.0f, 1.0f);
	GLint arc = glGetUniformLocation(program, "ambient_reflection_coef");
	GLint drc = glGetUniformLocation(program, "diffuse_reflection_coef");
	GLint src = glGetUniformLocation(program, "specular_reflection_coef");
	GLint se = glGetUniformLocation(program, "specular_exponent");
	glUniform1f(arc, 0.5f);
	glUniform1f(drc, 0.7f);
	glUniform1f(src, 6.0f);
	glUniform1f(se, 10.0f);
	GLint lsc = glGetUniformLocation(program, "light_source_color");
	GLint lsp = glGetUniformLocation(program, "light_source_position");
	glUniform4f(lsc, 1.0f, 1.0f, 0.0f, 1.0f);
	glUniform4f(lsp, 2.0f, 5.0f, 3.0f, 1.0f);
	GLint alc = glGetUniformLocation(program, "ambientLightColor");
	glUniform4f(alc, 0.5f, 0.5f, 0.5f, 1.0f);


}
