#ifndef CAMERA_H
#define CAMERA_H



#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Camera
{
public:


	glm::mat4 toWorld;
	glm::vec3 cameraPosition;
	glm::vec3 cameraLookAt;
	glm::vec3 cameraUp;

	Camera();
	~Camera();

};
#endif

