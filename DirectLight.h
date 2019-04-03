#include "Light.h"
class DirectLight : public Light
{
public:
	GLint colLoc, dirLoc;

	glm::vec3 direction;

	DirectLight(glm::vec3, glm::vec3);
	~DirectLight();

	void bind(GLuint);

	void rotate(float, glm::vec3);
};
