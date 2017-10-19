#version 150

// Phong fragment shader
//
// Contributor:  YOUR_NAME_HERE

// INCOMING DATA

// Add all variables containing data used by the
// fragment shader for lighting and shading



in vec4 color;

// OUTGOING DATA

// final fragment color
out vec4 fragmentColor;

void main()
{
    

	 
	fragmentColor = vec4( 1.0, 1.0, 1.0, 1.0 )*color;
}
