#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"
#include "object.h"

const int width = 800;
const int height = 600;

const float background[] = {0.647f, 0.533f, 0.333f};

float delta = 0.0f;
float last_frame = 0.0f;

double last_x = 0;
double last_y = 0;

bool pressed = false;
bool last_pressed = false;
bool click = false;
bool unclick = false;

glm::mat4 projection, view;


float unit_cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

float triangle[] = {
	0.0f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f
};

float unit_line[] = {
	0.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f
};

glm::vec3 coords[4] = {
	glm::vec3(0.5f, 0.0f, 0.5f),
	glm::vec3(-0.5f, 0.0f, 0.5f),
	glm::vec3(0.5f, 0.0f, -0.5f),
	glm::vec3(-0.5f, 0.0f, -0.5f)
	
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
glm::vec3 get_mouse_loc(GLFWwindow* window, float depth);
float mag(glm::vec3 vector);
void print_vec(glm::vec3 vector);

//remove
void processInput(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Slice Mice", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader object_shader("vertex.glsl", "fragment.glsl");
	Shader lamp_shader("lamp_vertex.glsl", "lamp_fragment.glsl");

	glm::vec3 ray_vector = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 lamp_pos = glm::vec3(1.0f, 1.0f, 1.0f);

	//								   fov					aspect ratio	  near   far
	projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

	//					    position				  target					up
	view = glm::lookAt(glm::vec3(5.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	Object obj(unit_cube, 36);
	Lamp lamp;
	Line ln(unit_line);


	lamp.translate(lamp_pos);

	lamp_shader.use();

	lamp_shader.setMat4("projection", projection);
	lamp_shader.setMat4("view", view);

	
	object_shader.use();
	
	object_shader.setVec3("lampColor", lamp.get_color());
	object_shader.setMat4("projection", projection);
	object_shader.setMat4("view", view);
	object_shader.setVec3("lightPos", lamp_pos);


	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		// calculating deltaT
		float current_frame = glfwGetTime();
		delta = current_frame - last_frame;
		last_frame = current_frame;

		last_pressed = pressed;
		pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
		click = (pressed && !last_pressed);
		unclick = (!pressed && last_pressed);

		processInput(window);

		glClearColor(background[0], background[1], background[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::vec3 ray_pos = get_mouse_loc(window, 3.0f);

		// calculates intersection of user controller ray with object
		if (obj.line_intersect(ray_pos, ray_vector)) {
			obj.color(glm::vec3(0.0f, 1.0f, 0.0f));
			// still unpins if plane ended in cube, but sends a failure message
			if (unclick) {
				ln.unpin(false);
			}
		} else {
			// cutting plane can only start and end outside of the cube
			if (click) {
				ln.pin();
			}
			if (unclick) {
				ln.unpin(true);
			}
			obj.color(glm::vec3(1.0f, 0.0f, 0.0f));
		}

		if (ln.is_cut_ready()) {
			std::pair<glm::vec3, glm::vec3> plane;
			plane = ln.cut_plane();
			obj.plane_intersect(plane.first, plane.second);
		}

		lamp.update();
		lamp.draw(lamp_shader);


		ln.update_pos(ray_pos);
		ln.update();
		ln.draw(lamp_shader);

		object_shader.use();
		object_shader.setVec3("lightPos", lamp_pos);

		obj.rotate(true);
		obj.update();
		obj.draw(object_shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
 }

glm::vec3 get_mouse_loc(GLFWwindow* window, float depth) {
	// obtains mouse screen coords
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// converts to normalized device coords (-1 to 1)
	float norm_x = (2 * xpos / width) - 1;
	float norm_y = (2 * ypos / height) - 1;

	// creates 
	glm::vec4 s_vec = glm::vec4(norm_x * depth, norm_y * depth, 0.0f, depth);
	s_vec = glm::inverse(projection) * s_vec;
	// resets w to 1.0f, depth data lost after going through projection matrix
	s_vec[3] = 1.0f;
	s_vec = glm::inverse(view) * s_vec;

	return glm::vec3(s_vec[0], -s_vec[1], s_vec[2]);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float mag(glm::vec3 vector) {
	// simple 3D pythagorean theorem calculator
	float a = vector[0];
	float b = vector[1];
	float c = vector[2];

	return pow(a * a + b * b + c * c, 0.5);
}

void print_vec(glm::vec3 vector) {
	std::cout << vector[0] << " " << vector[1] << " " << vector[2] << std::endl;
}
