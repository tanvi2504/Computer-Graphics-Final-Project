#version 150

// Phong fragment shader
//
// Contributor:  YOUR_NAME_HERE

// INCOMING DATA

// Add all variables containing data used by the
// fragment shader for lighting and shading

uniform vec4 light_source_color;

uniform vec4 ambient_material_color;
uniform float ambient_reflection_coef;
uniform vec4 diffuse_material_color;
uniform float diffuse_reflection_coef;
uniform vec4 specular_material_color;
uniform float specular_exponent;
uniform float specular_reflection_coef;
uniform vec4 ambientLightColor;

varying vec3 world_pos;
varying vec3 light;
varying vec3 world_norm;

// OUTGOING DATA

// final fragment color
out vec4 fragmentColor;

void main()
{
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
	vec4 specular = light_source_color*specular_material_color * specular_reflection_coef * pow(max(0.0, dot(R,V)), specular_exponent);

	 
	fragmentColor = vec4( 1.0, 1.0, 1.0, 1.0 )*(ambient + diffuse  + specular);
}
