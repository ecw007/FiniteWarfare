#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.


in float norms;
in vec3 TexCoords;
in vec3 Normal;
in vec3 Position;
in vec2 sniperTex;

uniform samplerCube skybox;
uniform sampler2D tex;


out vec4 vcolor;


vec4 calcLightColor(vec4 color) {
	vec4 lightColor = vec4(1.0f);
	vec3 lightDir = normalize(vec3(-20.0f, 30.0f, 1.0f));
	float diff = max(dot(Normal, lightDir), 0.0f);
	vec4 col = diff * color;
	return lightColor * col;
}

void main()
{
	if (norms == 1.0f) { //normal coloring
		vcolor = vec4(0.0f);
	}     
	else if (norms == 0.0f) {
		vcolor = texture(skybox, TexCoords);
	}
	else if (norms == 5.0f) {

		vec4 color = vec4(0.0f);

		if (Position.y <= 18) {
		//green yellow
			color = vec4(.5f, .7f, .3f, 0.0f);
		}
		else if (Position.y <= 26 && Position.y > 18) {
		// green
			color = vec4(.2f, 0.4, .1, 0.0f);
		}
		else if (Position.y <= 33 && Position.y > 26) {
		//light dark green
			color = vec4(.2f, .4, .2, 0.0f);
		}
		else if (Position.y > 33) {
		//dark green
			color = vec4(0.18f, 0.31f, 0.18f, 0.0f);
		}

		//APPLY LIGHT INFO
		vcolor = calcLightColor(color);
		
	}
	else if (norms == 8.0f) {
		vcolor = texture(tex, sniperTex);
	}
	else {
		vcolor = vec4(0.0f);
	}	
}
