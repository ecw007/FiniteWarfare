#ifndef SKYBOX_H
#define SKYBOX_H



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


class Skybox
{
public:
	vector<std::string> faces =
	{
		/*"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"*/
		"plains-of-abraham_rt.tga",
		"plains-of-abraham_lf.tga",
		"plains-of-abraham_up.tga",
		"plains-of-abraham_dn.tga",
		"plains-of-abraham_bk.tga",
		"plains-of-abraham_ft.tga"
	};

	
	

	const GLfloat skyboxVertices[8][3] = {
		// "Front" vertices
		{-250.0, -50.0,  250.0}, {250.0, -50.0,  250.0}, {250.0,  450.0,  250.0}, {-250.0,  450.0,  250.0},
		// "Back" vertices
		{-250.0, -50.0, -250.0}, {250.0, -50.0, -250.0}, {250.0,  450.0, -250.0}, {-250.0,  450.0, -250.0}
	};

	// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
	// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
	const GLuint indices[6][6] = {
		// Front face
		{0, 1, 2, 2, 3, 0},
		// Top face
		{1, 5, 6, 6, 2, 1},
		// Back face
		{7, 6, 5, 5, 4, 7},
		// Bottom face
		{4, 0, 3, 3, 7, 4},
		// Left face
		{4, 5, 1, 1, 0, 4},
		// Right face
		{3, 2, 6, 6, 7, 3}
	};

	unsigned int cubeMapTex;
	GLint uProjection, uView;
	GLuint skyboxVAO, VBO, EBO;
	GLint norm;


	static float typeColor;

	Skybox();
	~Skybox();

	unsigned int loadCubeMap(vector<string>);
	void draw(GLint);
};
#endif
