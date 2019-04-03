#include "window.h"
#include "Skybox.h"
#include "Lines.h"
//#include "Audio.h"
#include "Terrain.h"
//Audio* sound;
bool playSound;

bool first = true;

const char* window_title = "GLFW Starter Project";
GLint Window::shaderProgram;


Skybox* skybox;
OBJObject* test;
OBJObject* gun;
OBJObject* assault;
OBJObject* sniper;
Lines* cursor;
Terrain* terr;
int tLength;

bool forwardM, leftM, rightM, backwardM;

vector<OBJObject*> items;
vector<OBJObject*> spheres;

const double m_ROTSCALE = 10.0f;
bool leftMouse = false;
bool rightMouse = false;
glm::vec3 direction;

int seed = 1;
int timer = 0;
int mTimer = 0;

bool firstMouse = false;
bool firstC = true;


float nearDist;
float farDist;
float Hnear;
float ratio;
float Wnear;
float Hfar;
float Wfar;
glm::vec3 fc;
glm::vec3 nc;
glm::vec3 rightNormal;
glm::vec3 topNormal;
glm::vec3 leftNormal;
glm::vec3 botNormal;

int numTargets;

float prevH;

bool sphereRender = false;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 30.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

float lastx;
float lasty;

glm::vec3 lastP, currP;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{

	skybox = new Skybox();
	test = new OBJObject("eyeball_s.obj");
	test->setCenter(cam_look_at);

	assault = new OBJObject("assault.obj");
	assault->scale(0.12f);
	assault->setCenter(glm::vec3(1.8f, -1.2f, 26.5f));

	sniper = new OBJObject("sniper.obj");
	sniper->rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	sniper->rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
	sniper->scale(0.35f);
	sniper->setCenter(glm::vec3(1.8f, -1.2f, 26.5f));

	gun = sniper;

	cursor = new Lines();

	//sound = new Audio();
	playSound = true;

	srand(1);
	terr = new Terrain();
	tLength = sizeof(terr->terrain) / sizeof(terr->terrain[0]);

	//add targets
	for (int i = 0; i < 20; i++) {
		items.push_back(new OBJObject("target.obj"));
		items[i]->toWorld[3] = glm::vec4((rand() % tLength) - tLength / 2, 0.0f, (rand() % tLength) - tLength / 2, 1.0f);
		spheres.push_back(new OBJObject("sphere.obj"));
		spheres[i]->toWorld[3] = items[i]->toWorld[3];
		spheres[i]->toWorld[3][1] += 2.0f;
		spheres[i]->scale(3.0f);
		
	}
	numTargets = items.size();

	adjustHeight();
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	nearDist = 0.1f;
	farDist = 1000.0f;
	Hnear = 2.0f * tan(45.0f / 2.0f) * nearDist;
	ratio = (float)(Window::width) / (float)(Window::height);
	Wnear = Hnear * ratio;
	Hfar = 2.0f * tan(45.0f / 2.0f) * farDist;
	Wfar = Hfar * ratio;

	//alBufferData(buffer, format, buf, sizeS, freq);
	computeViewPlanes();

	//UI = new Screen();
	//Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	cout << "GAME START" << endl;
	cout << "Move: WASD, Stop Moving: SHIFT, Aim: Muose, Swap Weapons: SPACE, Change Map: T, Reset Map: R" << endl;
	cout << endl;
	cout << "TARGETS LEFT:" << numTargets << endl;
}

void Window::computeViewPlanes() {
	glm::vec3 dir = glm::normalize(cam_look_at - cam_pos);
	glm::vec3 right = glm::normalize(cross(dir, cam_up));

	//compute plane points
	fc = cam_pos + dir * farDist;
	nc = cam_pos + dir * nearDist;

	//compute plane normals
	glm::vec3 temp = glm::normalize((nc + right * Wnear / 2.0f) - cam_pos);
	rightNormal = cross(temp, cam_up);
	temp = glm::normalize((nc - right * Wnear / 2.0f) - cam_pos);
	leftNormal = cross(cam_up, temp);
	temp = glm::normalize((nc + cam_up * Hnear / 2.0f) - cam_pos);
	topNormal = cross(right, temp);
	temp = glm::normalize((nc - cam_up * Hnear / 2.0f) - cam_pos);
	botNormal = cross(temp, right);
}


// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	//delete UI;
	delete skybox;
	delete test;
	delete gun;
	delete cursor;
	delete terr;
	for (OBJObject* o : items) {
		delete o;
	}
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	//cout << width << ", " << height << endl;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		Hnear = 2.0f * tan(45.0f / 2.0f) * nearDist;
		ratio = (float)(Window::width) / (float)(Window::height);
		Wnear = Hnear * ratio;
		Hfar = 2.0f * tan(45.0f / 2.0f) * farDist;
		Wfar = Hfar * ratio;
	}
}

void Window::idle_callback()
{
	gun->updateArrBuff();
	cursor->update();
	if (timer > 50) {
		for (int i = 0; i < items.size(); i++) {
			OBJObject* o = items[i];
			if (!o->gotShot) {
				o->updateArrBuff();
			}
		}		
		timer = 0;
	}
	timer++;
	moveCamera();
	adjustTargetHeight();
	for (int i = 0; i < spheres.size(); i++) {
		spheres[i]->toWorld[3] = items[i]->toWorld[3];
		spheres[i]->toWorld[3][1] += 2.0f;
	}
}



glm::vec3 Window::trackBallMapping(double vecx, double vecy)
{
	glm::vec3 v;
	float d;
	v.x = (2.0*vecx - width) / width;
	v.y = (height - 2.0*vecy) / height;
	v.z = 0.0;
	d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	v = glm::normalize(v);         // Still need to normalize, since we only capped d, not v.
	return v;
}


void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {

	if (first) {
		first = false;
		lastx = xpos;
		lasty = ypos;
	}
	if (rightMouse) {
		float chgx = float(((xpos - lastx)) / 200.0f);
		float chgy = float(((ypos - lasty)) / 200.0f);
		glm::vec3 rotAxis = glm::vec3(0.0f);
		glm::mat4 change = glm::mat4(1.0f);
		if (abs(chgx) > abs(chgy)) {
			rotAxis = glm::vec3(0.0f, -1.0f, 0.0f);
			change = glm::rotate(glm::mat4(1.0f), chgx, rotAxis);
		}
		else {
			rotAxis = glm::normalize(cross(cam_look_at - cam_pos, cam_up));
			change = glm::rotate(glm::mat4(1.0f), chgy, -rotAxis);
		}

		glm::vec4 temp = glm::vec4(cam_pos, 0.0f);

		test->toWorld[3] -= temp;
		test->toWorld = change * test->toWorld;
		test->toWorld[3] += temp;
		cam_look_at = test->toWorld[3];

		assault->toWorld[3] -= temp;
		assault->toWorld = change * assault->toWorld;
		assault->toWorld[3] += temp;

		sniper->toWorld[3] -= temp;
		sniper->toWorld = change * sniper->toWorld;
		sniper->toWorld[3] += temp;

		cursor->toWorld[3] -= temp;
		cursor->toWorld = change * cursor->toWorld;
		cursor->toWorld[3] += temp;

		computeViewPlanes();
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}

	lastx = xpos;
	lasty = ypos;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	lastx = x;
	lasty = y;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		leftMouse = true;
		//sound->playShot();
		for (OBJObject* o : items) {
			if (!o->gotShot && (testShot(o->toWorld[3]))) {
				numTargets--;
				cout << "TARGETS LEFT:" << numTargets << endl;
				if (numTargets == 0) {
					cout << "GAME OVER! CONGRATULATIONS" << endl;
					cout << "REPLAY? PRESS ENTER" << endl;
				}
				o->gotShot = true;
				break;
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		leftMouse = false;
		lastx = x;
		lasty = y;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		rightMouse = true;
		lastx = x;
		lasty = y;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		rightMouse = false;

	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	// Render the cube

	skybox->draw(shaderProgram);
	//UI->draw();

	cursor->draw(shaderProgram);
	gun->draw(shaderProgram);
	terr->draw(shaderProgram);

	
	for (int i = 0; i < items.size(); i++) {
		if (!items[i]->gotShot) {
			if (testInside(items[i]->toWorld[3], 3.0f)) {
				if (sphereRender) {
					spheres[i]->draw(shaderProgram);
				}
				else {
					items[i]->draw(shaderProgram);
				}
			}
		}
	}
	
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{

		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			direction = glm::normalize(cam_look_at - cam_pos);
			forwardM = true;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			direction = -1.0f * glm::normalize(cam_look_at - cam_pos);
			backwardM = true;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			direction = glm::normalize(cam_look_at - cam_pos);
			direction = cross(direction, cam_up);
			rightM = true;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			direction = glm::normalize(cam_look_at - cam_pos);
			direction = -1.0f * cross(direction, cam_up);
			leftM = true;
		}

		if (key == GLFW_KEY_LEFT_SHIFT) {
			forwardM = false;
			backwardM = false;
			rightM = false;
			leftM = false;
		}
		if (key == GLFW_KEY_R) {
			seed = 1;
			srand(seed);
			delete terr;
			terr = new Terrain();
		}
		if (key == GLFW_KEY_T) {
			seed += 1;
			srand(seed);
			delete terr;
			terr = new Terrain();
		}
		if (key == GLFW_KEY_SPACE) {
			if (gun == assault) {
				gun = sniper;
			}
			else {
				gun = assault;
			}
		}
		if (key == GLFW_KEY_ENTER) {
			for (OBJObject* o : items) {
				o->gotShot = false;
			}
			cout << "PLAY AGAIN!" << endl;
		}
		if (key == GLFW_KEY_P) {
			if (playSound) {
				playSound = false;
				//sound->stopAudio();
			}
			else {
				playSound = true;
				//sound->startAudio();
			}
		}
		if (key == GLFW_KEY_H) {
			printVec(cam_pos);
		}
		if (key == GLFW_KEY_B) {
			sphereRender = !sphereRender;
		}
	}
}

void Window::printVec(glm::vec3 vec) {
	cout << vec.x << ", " << vec.y << ", " << vec.z << endl;
}

bool Window::testInside(glm::vec3 center, float radius) {
	computeViewPlanes();
	//test top
	glm::vec3 dir = glm::normalize(cam_look_at - cam_pos);
	float distance = glm::dot(center - cam_pos, topNormal);
	if (distance > radius) {
		return false;
	}

	//bottom
	distance = glm::dot(center - cam_pos, botNormal);
	if (distance > radius) {
		return false;
	}

	//left
	distance = glm::dot(center - cam_pos, leftNormal);
	if (distance > radius) {
		return false;
	}

	//right
	distance = glm::dot(center - cam_pos, rightNormal);
	if (distance > radius) {
		return false;
	}

	//near
	distance = glm::dot(center - nc, -dir);
	if (distance > radius) {
		return false;
	}

	//far
	distance = glm::dot(center - fc, dir);
	if (distance > radius) {
		return false;
	}
	return true;
}

int Window::adjustHeight() {
	int x = (int)(cam_pos.x) + tLength / 2;
	int z = (int)(cam_pos.z) + tLength / 2;
	if (x < 0 || x > tLength - 1) {
		return -1;
	}
	if (z < 0 || z > tLength - 1) {
		return -1;
	}
	float height = terr->terrain[x][z] + 4.5f;
	height -= prevH;
	cam_pos[1] += height;
	test->toWorld[3][1] += height;
	cam_look_at = test->toWorld[3];
	sniper->toWorld[3][1] += height;
	assault->toWorld[3][1] += height;
	cursor->toWorld[3][1] += height;
	prevH = terr->terrain[x][z] + 4.5f;
	return 1;
}

void Window::adjustTargetHeight() {

	for (OBJObject* o : items) {
		if (detectTargetBump(o)) {
			o->reverseChange();
			continue;
		}
		int x = (int)(o->toWorld[3][0]) + tLength / 2;
		int z = (int)(o->toWorld[3][2]) + tLength / 2;
		if (x < 0 || x > tLength - 1) {
			o->reverseChange();
			continue;
		}
		if (z < 0 || z > tLength - 1) {
			o->reverseChange();
			continue;
		}
		int height = terr->terrain[x][z] + 2;
		o->toWorld[3][1] = height;
	}
}

bool Window::testShot(glm::vec4 center) {
	if (testInside(center, 3.0f)) {
		glm::vec3 point = glm::vec3(center);
		point.y += 1.5f;
		glm::vec3 l1 = point - cam_pos;
		float dist = glm::length(l1);
		if (dist > 150.0f && gun == assault) {
			return false;
		}
		glm::vec3 l2 = cam_look_at - cam_pos;
		float angle = dot(l1, l2) / glm::length(l1) / glm::length(l2);
		angle = acos(angle);
		float ans = dist * sin(angle);
		//cout << ans << endl;
		if (gun == assault) {
			return (ans < 3.0f) || angle < 0.06f;
		}
		else {
			return (ans < 2.0f) || angle < 0.04f;
		}

	}
	return false;
}

void Window::moveCamera() {
	if (mTimer < 5) {
		mTimer++;
		return;
	}
	mTimer = 0;
	if (forwardM || backwardM || leftM || rightM) {
		direction[1] = 0;
		direction = glm::normalize(direction);
		cam_pos += direction;
		if (detectBump()) {
			cam_pos -= direction;
			forwardM = false;
			backwardM = false;
			leftM = false;
			rightM = false;
		}
		else if (adjustHeight() == -1) {
			cam_pos -= direction;		
			forwardM = false;
			backwardM = false;
			leftM = false;
			rightM = false;
		}
		else {
			test->toWorld[3] += glm::vec4(direction, 0.0f);
			cam_look_at = glm::vec3(test->toWorld[3]);
			assault->toWorld[3] += glm::vec4(direction, 0.0f);
			sniper->toWorld[3] += glm::vec4(direction, 0.0f);
			cursor->toWorld[3] += glm::vec4(direction, 0.0f);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
	}
}

bool Window::detectBump() {

	//cam_pos already moved
	for (OBJObject* o : items) {
		if (!o->gotShot) {
			float dist = glm::length(glm::vec3(o->toWorld[3]) - cam_pos);
			if (dist < 5.0f) {
				return true;
			}
		}
	}
	return false;
}


bool Window::detectTargetBump(OBJObject* item) {
	float dist = glm::length(glm::vec3(item->toWorld[3]) - cam_pos);
	if (dist < 5.0f) {
		return true;
	}
	
	for (OBJObject* o : items) {

		if (o == item) {
			continue;
		}

		if (!o->gotShot) {
			dist = glm::length(glm::vec3(o->toWorld[3] - item->toWorld[3]));
			if (dist < 6.0f) {
				return true;
			}
		}
	}
	return false;
}