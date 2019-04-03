#ifndef OBJOBJECT_H
#define OBJOBJECT_H

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

class OBJObject
{
private:
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;

	float angle;		//for spinnning OBJObject
	

public:

	glm::mat4 toWorld;
	glm::vec4 change;

	OBJObject();
	OBJObject(const char* filepath);
	~OBJObject();	

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, VBO2, VBO3;
	GLuint uProjection, uModelview, uToWorld, uCam, uView;
	GLint norm;

	bool target;
	bool gotShot;
	
	
	float typeColor;
	void parse(const char* filepath);
	void parseOther(const char* filepath);
	void parseSphere(const char* filepath);
	void setCenter(glm::vec3);
	void updateArrBuff();
	void arrBuffs();
	void draw(GLuint);
	void rotate(float, glm::vec3);
	void spin(float deg);		//spins OBJObject at speed based off deg
	void scale(float num);		//scales size of OBJObject based off num
	void findCenter();			//get vertex values for center of OBJObject
	void updateTarget();
	void reverseChange();
};

#endif