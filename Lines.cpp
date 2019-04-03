#include "Lines.h"
#include "Window.h"


float Lines::typeColor = 2.0f;

Lines::Lines()
{
	toWorld = glm::mat4(1.0f);
	toWorld[3] = glm::vec4(0.0f, 0.0f, 27.0f, 1.0f);


	curvePoints.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(0.05f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(-0.05f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(-0.1f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, 0.05f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, -0.05f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, -0.1f, 0.0f));
	
	
	/*curvePoints.push_back(glm::vec3(0.25f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(-0.1f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(-0.25f, 0.0f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, 0.25f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, -0.1f, 0.0f));
	curvePoints.push_back(glm::vec3(0.0f, -0.25f, 0.0f));*/
	for (int i = 0; i < 8; i++) {
		indices.push_back(i);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);


	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as curvePoints, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(glm::vec3), curvePoints.data(), GL_DYNAMIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(float), // Offset between consecutive curvePoints. Since each of our curvePoints have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the curvePoints array with anything.

	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those curvePoints? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}


Lines::~Lines()
{
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &VBO);

	glDeleteBuffers(1, &EBO);
}

void Lines::draw(GLint shaderProgram) {
	// Calculate the combination of the model and view (camera inverse) matrices
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uView = glGetUniformLocation(shaderProgram, "view");
	norm = glGetUniformLocation(shaderProgram, "norm");
	uToWorld = glGetUniformLocation(shaderProgram, "toWorld");
	glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &toWorld[0][0]);
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniform1f(norm, typeColor);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Lines::update() {
	
	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
// you want to draw, such as curvePoints, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(glm::vec3), curvePoints.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
}
