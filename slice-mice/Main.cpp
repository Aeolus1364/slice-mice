#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"

const int width = 800;
const int height = 600;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

float vertices2[] = {
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 0.0f,  1.0f, 0.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, int button, int action, int modifier);

class Object {
	unsigned int VAO, VBO;
	unsigned int m_num_verts;
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 translation = glm::vec3(0.0f);

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
	void update(Shader shader) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, translation);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		shader.setMat4("model", model);
	}
	void draw(Shader shader) {
		activate();
		update(shader);
		glDrawArrays(GL_TRIANGLES, 0, m_num_verts);
	}
	void set_translation(glm::vec3 trans) {
		translation = trans;
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


	Object test(vertices, 36);
	Object test2(vertices2, 3);


	shader.use();


	glm::mat4 projection, view;

	projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
	shader.setMat4("projection", projection);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		test.set_translation(glm::vec3(0.0f, 2.0f, 0.0f));
		
		/*test2.update(shader);

		test.update(shader);*/

		const float radius = 5.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		shader.setMat4("view", view);

		test.draw(shader);
		test2.draw(shader);

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
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {
		std::cout << "cringe" << std::endl;
	}
}
