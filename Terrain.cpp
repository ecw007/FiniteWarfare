#include "Terrain.h"
#include "Window.h"

int length = 513;
float Terrain::typeColor = 5.0f;

vector<int> triCount;
vector<glm::vec3> norms;

int i1, i2, i3;
glm::vec3 v1, v2, v3;

Terrain::Terrain()
{	
	center = glm::vec3(0.0f);
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			terrain[i][j] = 0;
		}
	}
	initializeTerrain();
	/*for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			cout << terrain[i][j] << " ";
		}
		cout << endl;
	}*/
	getVertNorms();		
	arrBuff();
}


Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &VBO);

	glDeleteBuffers(1, &EBO);

	vertices.clear();
	indices.clear();
	norms.clear();
}

void Terrain::initializeTerrain() {
	
	terrain[0][0] = 35;
	terrain[0][length-1] = 40;
	terrain[length-1][length-1] = 35;
	terrain[length-1][0] = 34;
	terrain[length / 2][length - 1] = 26;
	terrain[length / 2][0] = 22;
	terrain[0][length / 2] = 20; 
	terrain[length - 1][length / 2] = 25; 
	diamondSquare(length);
	for (int i = 1; i < length-1; i++) {
		for (int j = 1; j < length-1; j++) {
			int sum = terrain[i][j + 1] + terrain[i + 1][j] + terrain[i - 1][j] + terrain[i][j - 1];
			terrain[i][j] = sum / 4;
		}
	}
}

void Terrain::getVertNorms() {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			vertices.push_back(glm::vec3(i - length / 2, terrain[i][j], j - length / 2));
			triCount.push_back(0);
			norms.push_back(glm::vec3(0.0f));
		}
	}
	for (int i = 0; i < length - 1; i++) {
		for (int j = 0; j < length - 1; j++) {

			i1 = i * length + j;
			i2 = i * length + j + 1;
			i3 = (i + 1)*length + j;
			v1 = vertices[i1];
			v2 = vertices[i2];
			v3 = vertices[i3];
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
			//compute normal
			glm::vec3 normal = glm::normalize(cross(v2 - v1, v3 - v1));
			triCount[i1]++;
			triCount[i2]++;
			triCount[i3]++;
			norms[i1] += normal;
			norms[i2] += normal;
			norms[i3] += normal;



			i1 = (i + 1)*length + j;
			i2 = i * length + j + 1;
			i3 = (i + 1)*length + j + 1;
			v1 = vertices[i1];
			v2 = vertices[i2];
			v3 = vertices[i3];
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);

			normal = glm::normalize(cross(v2 - v1, v3 - v1));
			triCount[i1]++;
			triCount[i2]++;
			triCount[i3]++;
			norms[i1] += normal;
			norms[i2] += normal;
			norms[i3] += normal;
		}
	}
	
	for (int i = 0; i < norms.size(); i++) {
		norms[i] = norms[i] / (float)triCount[i];
	}
}

void Terrain::diamondSquare(int size) {
	int half = size / 2;
	if (half < 1)
		return;
	//square steps
	for (int z = half; z < length; z += size)
		for (int x = half; x < length; x += size)
			square(x % length, z % length, half);
	// diamond steps
	int col = 0;
	for (int x = 0; x < length; x += half)
	{
		col++;
		//If this is an odd column.
		if (col % 2 == 1)
			for (int z = half; z < length; z += size)
				diamond(x % length, z % length, half);
		else
			for (int z = 0; z < length; z += size)
				diamond(x % length, z % length, half);
	}
	diamondSquare(size / 2);
}

void Terrain::diamond(int x, int z, int change) {
	int count = 0;
	float avg = 0.0f;
	if (x - change >= 0)
	{
		avg += terrain[x - change][z];
		count++;
	}
	if (x + change < length)
	{
		avg += terrain[x + change][z];
		count++;
	}
	if (z - change >= 0)
	{
		avg += terrain[x][z - change];
		count++;
	}
	if (z + change < length)
	{
		avg += terrain[x][z + change];
		count++;
	}
	
	avg /= count;
	avg += random(change);
	
	terrain[x][z] += (int)avg;
	
}

void Terrain::square(int x, int z, int change) {
	int count = 0;
	float avg = 0.0f;
	if (x - change >= 0 && z - change >= 0)
	{
		avg += terrain[x - change][z - change];
		count++;
	}
	if (x - change >= 0 && z + change < length)
	{
		avg += terrain[x - change][z + change];
		count++;
	}
	if (x + change < length && z - change >= 0)
	{
		avg += terrain[x + change][z - change];
		count++;
	}
	if (x + change < length && z + change < length)
	{
		avg += terrain[x + change][z + change];
		count++;
	}
	
	avg /= count;
	avg += random(change);
	
	//cout << avg << ", " << count << endl;
	terrain[x][z] += avg;
	
}

int Terrain::random(int change) {
	int x = (rand() % (change));
	//cout << x << endl;
	return x * -0.1;
}

void Terrain::draw(GLint shaderProgram) {
	glm::mat4 toWorld = glm::mat4(1.0f);
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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Terrain::arrBuff() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);


	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as curvePoints, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(float), // Offset between consecutive curvePoints. Since each of our curvePoints have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the curvePoints array with anything.

	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those curvePoints? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), norms.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(float), // Offset between consecutive vertices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}