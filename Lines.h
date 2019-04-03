#ifndef LINES_H
#define LINES_H



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


class Lines
{
public:

	GLuint VBO, VAO, EBO, VBO2;
	GLuint uProjection, uModelview, uToWorld, uCam, uView;
	GLint norm;

	glm::mat4 toWorld;
	static glm::mat4x4 basis;
	glm::mat4x4 matrix;
	vector<glm::vec3*> curveVec;
	
	vector<glm::vec3> curvePoints;
	vector<int> indices;

	static float typeColor;
	float totalLength;
	int interval;
	int currInterval;

	Lines();
	~Lines();

	void draw(GLint);
	void update();
};
#endif
