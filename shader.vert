


#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec3 skyboxV;
layout (location = 3) in vec3 lineVec;
layout (location = 4) in vec3 terrVec;
layout (location = 5) in vec3 terrNorms;
layout (location = 6) in vec2 texCord;


// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 toWorld;
uniform float norm;


// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.

out vec2 sniperTex;
out vec3 TexCoords;
out float norms;
out vec3 Normal;
out vec3 Position;


void main()
{
	
	if (norm == 1.0f) {
		Normal = normalize(mat3(transpose(inverse(toWorld))) * normals);
		Position = vec3(toWorld * vec4(position, 1.0));
		gl_Position = projection * view * toWorld * vec4(position, 1.0);
	}
	else if (norm == 0.0f) {
		 TexCoords = skyboxV;
		 gl_Position = projection * view * vec4(skyboxV, 1.0);
	}
	else if (norm == 2.0f) {
		 gl_Position = projection * view * toWorld * vec4(lineVec, 1.0);
	}
	else if (norm == 5.0f) {
		gl_Position = projection * view * toWorld * vec4(terrVec, 1.0);
		Position = terrVec;
		Normal = normalize(mat3(transpose(inverse(toWorld))) * terrNorms);
	}
	else if (norm == 8.0f) {
		
		gl_Position = projection * view * toWorld * vec4(position, 1.0);
		sniperTex = texCord;
	}
	else {
		gl_Position = projection * view * toWorld * vec4(position, 1.0);
	}
	norms = norm;
	
}  
