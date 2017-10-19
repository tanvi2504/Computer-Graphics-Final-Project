#version 150

// Texture mapping fragment shader
//
// Contributor:  YOUR_NAME_HERE

// INCOMING DATA

// Add all variables containing data used by the
// fragment shader for lighting and texture mapping
in vec2 texCoord;
uniform sampler2D smiley;


// OUTGOING DATA

// final fragment color
out vec4 fragmentColor;

void main()
{
    // Replace with proper texture mapping code
	//apply two different images for the front and back face
	//if (gl_FrontFacing){
	//texture access in Shader
    fragmentColor = texture(smiley, texCoord);
	
}
