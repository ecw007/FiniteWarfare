#include "DirectLight.h" 

DirectLight::DirectLight(glm::vec3 col, glm::vec3 pos) {
	toWorld = glm::translate(glm::mat4(1.0f), pos);
	//toWorld = glm::mat4(1.0f);
	color = col;
	direction = -toWorld[3];
}

DirectLight::~DirectLight() {

}

void DirectLight::bind(GLuint shaderProgram) {
	colLoc = glGetUniformLocation(shaderProgram, "Directlight.color");
	glUniform3fv(colLoc, 1, &color[0]);
	dirLoc = glGetUniformLocation(shaderProgram, "Directlight.direction");
	glUniform3fv(dirLoc, 1, &direction[0]);
}

void DirectLight::rotate(float angle, glm::vec3 vec) {

	this->toWorld = glm::rotate(glm::mat4(1.0f), angle, vec) * this->toWorld;
	direction = -toWorld[3];
	//cout << angle << endl;
	//cout << vec.x << " " << vec.y << " " << vec.z << endl;
	//cout << position.x << " " << position.y << " " << position.z << endl;
}