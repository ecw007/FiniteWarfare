#include "OBJObject.h"
#include "Window.h"
#include "Texture.h"



float centerx;	//class variables for center of OBJObject
float centery;
float centerz;



OBJObject::OBJObject()
{
}

OBJObject::OBJObject(const char *filepath)
{
	typeColor = 1.0f;
	if (strcmp(filepath, "sniper.obj") == 0) {
		typeColor = 8.0f;
	}
	toWorld = glm::mat4(1.0f);	
	//get vertices and vertex normals and store in vectors
	gotShot = false;
	if (strcmp(filepath, "target.obj") == 0) {
		parseOther(filepath);
		target = true;
	}
	else if (strcmp(filepath, "sphere.obj") == 0) {
		parseSphere(filepath);
		target = false;
	}
	else {
		parse(filepath);
		target = false;
	}
	//set angle for spin function
	angle = 0.0f;
	//get center vertex values
	findCenter();
	
	
	arrBuffs();
}


OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);	
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	if (typeColor == 8.0f) {
		glDeleteBuffers(1, &VBO3);
	}
	glDeleteBuffers(1, &EBO);
	
}

//read file and store vertices and vertex normals in vectors
void OBJObject::parse(const char *filepath) 
{
	std::vector<glm::vec3> verticesTemp;
	std::vector<glm::vec3> normalsTemp;
	std::vector<glm::vec2> textureTemp;
	std::vector<int> verticesInd;
	std::vector<int> normalsInd;
	std::vector<int> textureInd;
	FILE* fp;
	float x, y, z;
	int c1, c2;
	int v1, v2, v3;
	int n1, n2, n3;
	int t1, t2, t3;

	fp = fopen(filepath, "rb");

	if (fp == NULL) {
		cerr << "error loading file" << endl;
	}

	c1 = fgetc(fp);

	while (c1 != EOF) {			//continue until end of file

		if (c1 == 'v') {		//only read lines starting with v
			c2 = fgetc(fp);
			if (c2 == ' ') {	//vertices
				fscanf(fp, " %f %f %f", &x, &y, &z);
				glm::vec3* vec = new glm::vec3(x, y, -z);
				verticesTemp.push_back(*vec);
			}
			else if (c2 == 'n') {	//normals
				fscanf(fp, " %f %f %f", &x, &y, &z);
				glm::vec3* vec = new glm::vec3(x, y, z);
				normalsTemp.push_back(*vec);
			}
			else if (typeColor == 8.0f && c2 == 't') {
				fscanf(fp, " %f %f", &x, &y);
				glm::vec2* vec = new glm::vec2(x, y);
				textureTemp.push_back(*vec);
			}
			
		}
		else if (c1 == 'f') {
			c2 = fgetc(fp);
			if (c2 = ' ') {
				fscanf(fp, "%i/%i/%i %i/%i/%i %i/%i/%i", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
				verticesInd.push_back(v1 - 1);
				verticesInd.push_back(v2 - 1);
				verticesInd.push_back(v3 - 1);
				normalsInd.push_back(n1 - 1);
				normalsInd.push_back(n2 - 1);
				normalsInd.push_back(n3 - 1);
				if (typeColor == 8.0f) {
					textureInd.push_back(t1 - 1);
					textureInd.push_back(t2 - 1);
					textureInd.push_back(t3 - 1);
				}
			}
		}
		c1 = fgetc(fp);
	}

	for (int i = 0; i < verticesInd.size(); i++) {
		vertices.push_back(verticesTemp[verticesInd[i]]);
		normals.push_back(normalsTemp[normalsInd[i]]);
		if (typeColor == 8.0f) {
			textures.push_back(textureTemp[textureInd[i]]);
		}
		indices.push_back(i);
	}
	fclose(fp);
}

void OBJObject::parseOther(const char *filepath)
{
	std::vector<glm::vec3> verticesTemp;
	std::vector<glm::vec3> normalsTemp;
	std::vector<int> verticesInd;
	std::vector<int> normalsInd;
	FILE* fp;
	float x, y, z;
	int c1, c2;
	int v1, v2, v3, v4;
	int n1, n2, n3, n4;
	int t1, t2, t3, t4;

	fp = fopen(filepath, "rb");

	if (fp == NULL) {
		cerr << "error loading file" << endl;
	}

	c1 = fgetc(fp);

	while (c1 != EOF) {			//continue until end of file

		if (c1 == 'v') {		//only read lines starting with v
			c2 = fgetc(fp);
			if (c2 == ' ') {	//vertices
				fscanf(fp, " %f %f %f", &x, &y, &z);
				glm::vec3* vec = new glm::vec3(x, y, -z);
				verticesTemp.push_back(*vec);

			}
			else if (c2 == 'n') {	//normals
				fscanf(fp, " %f %f %f", &x, &y, &z);
				glm::vec3* vec = new glm::vec3(x, y, z);
				normalsTemp.push_back(*vec);
			}

		}
		else if (c1 == 'f') {
			c2 = fgetc(fp);
			if (c2 = ' ') {
				fscanf(fp, "%i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3, &v4, &t4, &n4);
				verticesInd.push_back(v1 - 1);
				verticesInd.push_back(v2 - 1);
				verticesInd.push_back(v3 - 1);
				verticesInd.push_back(v4 - 1);
				normalsInd.push_back(n1 - 1);
				normalsInd.push_back(n2 - 1);
				normalsInd.push_back(n3 - 1);
				normalsInd.push_back(n4 - 1);
			}
		}
		c1 = fgetc(fp);
	}

	for (int i = 0; i < verticesInd.size(); i++) {
		vertices.push_back(verticesTemp[verticesInd[i]]);
		normals.push_back(normalsTemp[normalsInd[i]]);
		indices.push_back(i);
	}
	fclose(fp);
}

void OBJObject::parseSphere(const char* filepath) {
	std::vector<glm::vec3> verticesTemp;
	std::vector<glm::vec3> normalsTemp;
	std::vector<int> verticesInd;
	std::vector<int> normalsInd;
	FILE* fp;
	float x, y, z;
	int c1, c2;
	int v1, v2, v3;
	int n1, n2, n3;

	fp = fopen(filepath, "rb");

	if (fp == NULL) {
		cerr << "error loading file" << endl;
	}

	c1 = fgetc(fp);

	while (c1 != EOF) {			//continue until end of file

		if (c1 == 'v') {		//only read lines starting with v
			c2 = fgetc(fp);
			if (c2 == ' ') {	//vertices
				fscanf(fp, " %f %f %f", &x, &y, &z);
				glm::vec3* vec = new glm::vec3(x, y, -z);
				verticesTemp.push_back(*vec);

			}
			else if (c2 == 'n') {	//normals
				fscanf(fp, " %f %f %f", &x, &y, &z);
				glm::vec3* vec = new glm::vec3(x, y, z);
				normalsTemp.push_back(*vec);
			}

		}
		else if (c1 == 'f') {
			c2 = fgetc(fp);
			if (c2 = ' ') {
				fscanf(fp, "%i//%i %i//%i %i//%i", &v1, &n1, &v2, &n2, &v3, &n3);
				verticesInd.push_back(v1 - 1);
				verticesInd.push_back(v2 - 1);
				verticesInd.push_back(v3 - 1);
				normalsInd.push_back(n1 - 1);
				normalsInd.push_back(n2 - 1);
				normalsInd.push_back(n3 - 1);
			}
		}
		c1 = fgetc(fp);
	}

	for (int i = 0; i < verticesInd.size(); i++) {
		vertices.push_back(verticesTemp[verticesInd[i]]);
		normals.push_back(normalsTemp[normalsInd[i]]);
		indices.push_back(i);
	}
	fclose(fp);
}

void OBJObject::setCenter(glm::vec3 cent) {
	toWorld[3] = glm::vec4(cent, 1.0f);
	updateArrBuff();
}

void OBJObject::updateArrBuff() {
	if (target) {
		updateTarget();
	}
	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.

}

void OBJObject::arrBuffs() {
	
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	if (typeColor == 8.0f) {
		glGenBuffers(1, &VBO3);
	}
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);
	

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(float), // Offset between consecutive vertices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(float), // Offset between consecutive vertices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.



	if (typeColor == 8.0f) {
		Texture::loadTexture();
		glBindBuffer(GL_ARRAY_BUFFER, VBO3);
		// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
		// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
		glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), textures.data(), GL_STATIC_DRAW);
		// Enable the usage of layout location 0 (check the vertex shader to see what this is)
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			2, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
			2 * sizeof(float), // Offset between consecutive vertices. Since each of our vertices have 3 floats, they should have the size of 3 f
			(GLvoid*)0);
	}

	
	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

void OBJObject::draw(GLuint shaderProgram) 
{
	
	// Calculate the combination of the model and view (camera inverse) matrices
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uView = glGetUniformLocation(shaderProgram, "view");
	uToWorld = glGetUniformLocation(shaderProgram, "toWorld");
	glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &toWorld[0][0]);
	norm = glGetUniformLocation(shaderProgram, "norm");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniform1f(norm, typeColor);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	if (target) {
		glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	
}

//spin OBJobject based off deg, copied from cube::spin
void OBJObject::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the cube
	this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

//scale OBJObject based off num, multiply all vertices by num
void OBJObject::scale(float num) {
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] *= num;
	}
	updateArrBuff();
}


void OBJObject::rotate(float angle, glm::vec3 vec) {
	this->toWorld = glm::rotate(glm::mat4(1.0f), angle, vec)*(this->toWorld);
}

//find and store center vertex values 
void OBJObject::findCenter() {

	float minx = vertices[0].x;
	float miny = vertices[0].y;
	float minz = vertices[0].z;
	float maxx = vertices[0].x;
	float maxy = vertices[0].y;
	float maxz = vertices[0].z;

	//find min and max values for x, y, z dimensions
	for (unsigned int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x < minx) {
			minx = vertices[i].x;
		}
		else if (vertices[i].x > maxx) {
			maxx = vertices[i].x;
		}
		if (vertices[i].y < miny) {
			miny = vertices[i].y;
		}
		else if (vertices[i].y > maxy) {
			maxy = vertices[i].y;
		}
		if (vertices[i].z < minz) {
			minz = vertices[i].z;
		}
		else if (vertices[i].z > maxz) {
			maxz = vertices[i].z;
		}
	}

	//store center in class variable
	centerx = (minx + maxx) / 2.0f;
	centery = (miny + maxy) / 2.0f;
	centerz = (minz + maxz) / 2.0f;

	for (unsigned int k = 0; k < vertices.size(); k++) {
		vertices[k].x -= centerx;
		vertices[k].y -= centery;
		vertices[k].z -= centerz;
	}	
}

void OBJObject::updateTarget() {
	change = glm::vec4(0.0f);
	int x = rand() % 2;
	if (x == 0) {
		change[0] += 1.0f;
	}
	else {
		change[2] += 1.0f;
	}
	x = rand() % 2;
	if (x == 0) {
		change *= -1;
	}
	toWorld[3] += change;
}

void OBJObject::reverseChange() {
	toWorld[3] -= change;
}

