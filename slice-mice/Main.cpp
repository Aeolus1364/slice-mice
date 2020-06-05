#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"

const int width = 800;
const int height = 600;

const float background[] = {0.647f, 0.533f, 0.333f};

float delta = 0.0f;
float last_frame = 0.0f;

double last_x = 0;
double last_y = 0;

float unit_cube[] = {
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,

		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, int button, int action, int modifier);
void processInput(GLFWwindow* window);

class Object {
	unsigned int VAO, VBO;
	unsigned int m_num_verts;
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 translation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

public:
	Object(float * verts, unsigned int num_verts) {
		m_num_verts = num_verts;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		activate();

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_num_verts * 3 * sizeof(float), verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	void activate() {
		glBindVertexArray(VAO);
	}
	void update() {
		model = glm::mat4(1.0f);

		model = glm::translate(model, translation);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3((float)glfwGetTime() * 0.05));

	}
	void draw(Shader shader) {
		activate();
		shader.setMat4("model", model);
		shader.setVec3("color", glm::vec3(0.463, 0.275, 0.137));
		glDrawArrays(GL_TRIANGLES, 0, m_num_verts);
	}
	void translate(float x, float y, float z) {
		translation = glm::vec3(x, y, z);
	}
	glm::mat4 get_model() {
		return model;
	}
};


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
	glfwSetMouseButtonCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader shader("vertex.glsl", "fragment.glsl");


	Object test(unit_cube, 36);


	shader.use();

	glm::mat4 projection, view;

	//								   fov					aspect ratio	  near   far
	projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
	//projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);
	shader.setMat4("projection", projection);

	//					    position				  target					up
	view = glm::lookAt(glm::vec3(5.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	shader.setMat4("view", view);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		// calculating deltaT
		float current_frame = glfwGetTime();
		delta = current_frame - last_frame;
		last_frame = current_frame;

		processInput(window);

		glClearColor(background[0], background[1], background[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		test.translate(1.5, 0.0, 0.0);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (last_x != xpos || last_y != ypos) {
			//std::cout << xpos << ", " << ypos << std::endl;
		}
		last_x = xpos;
		last_y = ypos;

		float norm_x = (2 * xpos / width) - 1;
		float norm_y = (2 * ypos / height) - 1;


		/*glm::vec4 s_vec = glm::vec4(norm_x, norm_y, 0.0f, 1.0f);

		std::cout << s_vec[0] << ", " << s_vec[1] << ", " << s_vec[2] << ", " << s_vec[3] << std::endl;

		s_vec = glm::inverse(projection) * s_vec;
		std::cout << s_vec[0] << ", " << s_vec[1] << ", " << s_vec[2] << ", " << s_vec[3] << std::endl;

		s_vec = glm::inverse(view) * s_vec;
		std::cout << s_vec[0] << ", " << s_vec[1] << ", " << s_vec[2] << ", " << s_vec[3] << std::endl;
		std::cout << std::endl;*/

		

		glm::vec4 testing = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		std::cout << testing[0] << ", " << testing[1] << ", " << testing[2] << ", " << testing[3] << std::endl;

		testing = test.get_model() * testing;
		std::cout << testing[0] << ", " << testing[1] << ", " << testing[2] << ", " << testing[3] << std::endl;

		testing = view * testing;
		std::cout << testing[0] << ", " << testing[1] << ", " << testing[2] << ", " << testing[3] << std::endl;


		testing = projection * testing;
		std::cout << testing[0] << ", " << testing[1] << ", " << testing[2] << ", " << testing[3] << std::endl;

		
		glm::vec3 bruh = glm::vec3(testing[0] / testing[3], testing[1] / testing[3], testing[2] / testing[3]);

		float x = bruh[0];
		float y = bruh[1];

		std::cout << x << ", " << y << std::endl;

		x += 1;
		y += 1;
		
		x *= width / 2;
		y *= height / 2;

		std::cout << x << ", " << y << std::endl;


		std::cout << std::endl;



		test.update();
		test.draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
 }


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, int button, int action, int modifier) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	std::cout << xpos << ", " << ypos << std::endl;

	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {
		std::cout << "cringe" << std::endl;
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}