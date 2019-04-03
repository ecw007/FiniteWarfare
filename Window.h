#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "shader.h"
#include "OBJObject.h"



class Window
{
public:
	static int width;

	static GLint shaderProgram;
	
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void computeViewPlanes();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static glm::vec3 trackBallMapping(double, double);
	static void cursor_pos_callback(GLFWwindow*, double, double);
	static void mouse_button_callback(GLFWwindow*, int, int, int);
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void printVec(glm::vec3);
	static bool testInside(glm::vec3, float);
	static int adjustHeight();
	static void adjustTargetHeight();
	static bool testShot(glm::vec4);
	static void moveCamera();
	static bool detectBump();
	static bool detectTargetBump(OBJObject*);
};


#endif
