#version 150

// Phong vertex shader
//
// Contributor:  YOUR_NAME_HERE

// INCOMING DATA

// Vertex location (in model space)
in vec4 vPosition;

// Normal vector at vertex (in model space)
in vec3 vNormal;

// Model transformations
uniform vec3 theta;
uniform vec3 trans;
uniform vec3 scale;

// Camera parameters
uniform vec3 cPosition;
uniform vec3 cLookAt;
uniform vec3 cUp;

// View volume boundaries
uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;

// Add all necessary variables for doing the
// vertex shader portion of lighting and shading here

uniform vec4 light_source_position;
uniform vec4 light_source_color;

uniform vec4 ambient_material_color;
uniform float ambient_reflection_coef;
uniform vec4 diffuse_material_color;
uniform float diffuse_reflection_coef;
uniform vec4 specular_material_color;
uniform float specular_exponent;
uniform float specular_reflection_coef;
uniform vec4 ambientLightColor;

// OUTGOING DATA
//will be sent to the fragment shader
out vec4 color;

// Add all necessary variables for communicating with
// the fragment shader here

void main()
{
    // Compute the sines and cosines of each rotation about each axis
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    // Create rotation matrices
    mat4 rxMat = mat4( 1.0,  0.0,  0.0,  0.0,
                       0.0,  c.x,  s.x,  0.0,
                       0.0,  -s.x, c.x,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 ryMat = mat4( c.y,  0.0,  -s.y, 0.0,
                       0.0,  1.0,  0.0,  0.0,
                       s.y,  0.0,  c.y,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 rzMat = mat4( c.z,  s.z,  0.0,  0.0,
                       -s.z, c.z,  0.0,  0.0,
                       0.0,  0.0,  1.0,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 xlateMat = mat4( 1.0,     0.0,     0.0,     0.0,
                          0.0,     1.0,     0.0,     0.0,
                          0.0,     0.0,     1.0,     0.0,
                          trans.x, trans.y, trans.z, 1.0 );

    mat4 scaleMat = mat4( scale.x,  0.0,     0.0,     0.0,
                          0.0,      scale.y, 0.0,     0.0,
                          0.0,      0.0,     scale.z, 0.0,
                          0.0,      0.0,     0.0,     1.0 );

    // Create view matrix
    vec3 nVec = normalize( cPosition - cLookAt );
    vec3 uVec = normalize( cross (normalize(cUp), nVec) );
    vec3 vVec = normalize( cross (nVec, uVec) );

    mat4 viewMat = mat4( uVec.x, vVec.x, nVec.x, 0.0,
                         uVec.y, vVec.y, nVec.y, 0.0,
                         uVec.z, vVec.z, nVec.z, 0.0,
                         -1.0*(dot(uVec, cPosition)),
                         -1.0*(dot(vVec, cPosition)),
                         -1.0*(dot(nVec, cPosition)), 1.0 );

    // Create projection matrix
    mat4 projMat = mat4( (2.0*near)/(right-left), 0.0, 0.0, 0.0,
                         0.0, ((2.0*near)/(top-bottom)), 0.0, 0.0,
                         ((right+left)/(right-left)),
                         ((top+bottom)/(top-bottom)),
                         ((-1.0*(far+near)) / (far-near)), -1.0,
                         0.0, 0.0, ((-2.0*far*near)/(far-near)), 0.0 );

    // Transformation order:
    //    scale, rotate Z, rotate Y, rotate X, translate
    mat4 modelMat = xlateMat * rxMat * ryMat * rzMat * scaleMat;
    mat4 modelViewMat = viewMat * modelMat;

	//we only need the scaling and rotation part
	mat3 upper =mat3(modelViewMat);

    // Transform the vertex location into clip space
    gl_Position =  projMat * viewMat  * modelMat * vPosition;

	mat3 normalMat = transpose(inverse(upper));
	mat4 m = transpose(inverse(modelViewMat));

    // Add all code here to do the lighting-related calculations
    // that must be done in the vertex shader

	vec4 vertexEyeSpace = (modelViewMat * vPosition);    //current vertex is transformed to eye space
	vec3 normalEyeSpace = normalize(normalMat * vNormal);
	vec4 lightSource =  viewMat*light_source_position * vec4(0.0, 0.16, 0.0, 0.0); // calculate the light position
	
	//world space
	vec3 world_pos = (vertexEyeSpace).xyz;
	vec3 light = (lightSource).xyz;
	vec3 world_norm = normalEyeSpace;

	// Add all necessary code to implement the
    // fragment shader portion of Phong shading
    vec3 N = normalize (world_norm);
	vec3 L = normalize (light - world_pos);//light direction
	//which computes the reflected direction
	//incident ray(I) is not reversed, so make it negative  -reflect(L,N).
	vec3 R = normalize(reflect(-L,N));
	vec3 V = -normalize(world_pos);//view direction
    
	//ambient
	vec4 ambient = ambient_material_color * ambient_reflection_coef * ambientLightColor;

	// diffuse
	vec4 diffuse = light_source_color*diffuse_material_color * diffuse_reflection_coef * (dot(N, L));
	

	// specular
	vec4 specular = specular_material_color * specular_reflection_coef * pow(max(0.0, dot(R,V)), specular_exponent);

	color = (ambient + diffuse  + specular);

	

}
