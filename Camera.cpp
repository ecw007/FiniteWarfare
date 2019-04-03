#include "Camera.h"


Camera::Camera()
{
	cameraPosition = glm::vec3(0.0f, 0.0f, 30.0f);
	cameraLookAt = glm::vec3(0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	toWorld = glm::mat4(1.0f);
	//toWorld[3] = glm::vec4(0.0f, 0.0f, 30.0f, 1.0f);
}


Camera::~Camera()
{
}
