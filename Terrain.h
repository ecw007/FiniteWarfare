#ifndef TERRAIN_H
#define TERRAIN_H

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

class Terrain
{
public:
	int terrain[513][513];
	vector<glm::vec3> vertices;
	vector<int> indices;
	glm::vec3 center;

	GLuint VBO, VAO, EBO, VBO2;
	GLuint uProjection, uModelview, uToWorld, uCam, uView;
	GLint norm;

	static float typeColor;

	Terrain();
	~Terrain();
	void initializeTerrain();
	void getVertNorms();
	void diamondSquare(int size);
	void diamond(int, int, int);
	void square(int, int, int);
	int random(int);
	void draw(GLint);
	void arrBuff();
};
#endif
