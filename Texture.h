#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts

using namespace std; // allow console printouts without std::

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

class Texture
{
public:
	static unsigned char* loadPPM(const char* filename, int& width, int& height);
	static void loadTexture();
};

#endif
