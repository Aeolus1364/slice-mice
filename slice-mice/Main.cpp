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

bool pause = false;

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

float unit_cube_no_normals[] = {
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

float triangle[] = {
	0.0f, 1.0f, 0.5f,
	0.0f, 0.0f, 0.5f,
	0.0f, 0.0f, -0.5f
};

float unit_line[] = {
	0.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, int button, int action, int modifier);
glm::vec3 get_mouse_loc(GLFWwindow* window, float depth);
float mag(glm::vec3 vector);

//remove
void processInput(GLFWwindow* window);

class Triangle {
	glm::vec4 m_verts[3];
	glm::mat4* m_model;
	glm::vec3 m_intersection;

public:
	Triangle() {

	}
	Triangle(float* vertices, glm::mat4* model) {
		m_verts[0] = glm::vec4(vertices[0], vertices[1], vertices[2], 0.0f);
		m_verts[1] = glm::vec4(vertices[3], vertices[4], vertices[5], 0.0f);
		m_verts[2] = glm::vec4(vertices[6], vertices[7], vertices[8], 0.0f);

		m_model = model;

		m_intersection = glm::vec3(0.0f);
	}
	bool ray_intersection(glm::vec3 ray_pos, glm::vec3 ray_vector) {
		// converting from local coordinates to world coordinates
		glm::vec3 a_world = *m_model * m_verts[0];
		glm::vec3 b_world = *m_model * m_verts[1];
		glm::vec3 c_world = *m_model * m_verts[2];

		// calculates the vectors for each side of the triangle
		glm::vec3 ab = b_world - a_world;
		glm::vec3 ca = a_world - c_world;
		glm::vec3 bc = c_world - b_world;

		// normal vector calculated by crossing two triangle vectors
		glm::vec3 n = glm::cross(ca, ab);

		// w is the vector from the ray's to the point defining the plane
		glm::vec3 w = a_world - ray_pos;

		// calculates the ray's intersection with the plane
		glm::vec3 intersect = ray_pos + (glm::dot(w, n) / glm::dot(ray_vector, n)) * ray_vector;

		// calculates vectors pointing from the intersection point to each point on the triangle
		glm::vec3 ia = intersect - a_world;
		glm::vec3 ib = intersect - b_world;
		glm::vec3 ic = intersect - c_world;

		// calculates 3 values that are positive or negative based on which side of each line the point is on
		float side_ab = glm::dot(glm::cross(ab, ia), n);
		float side_bc = glm::dot(glm::cross(bc, ib), n);
		float side_ca = glm::dot(glm::cross(ca, ic), n);

		// saves intersect point
		m_intersection = intersect;

		// if the values are positive for every side, the point is inside the triangle
		return (side_ab >= 0 && side_bc >= 0 && side_ca >= 0);
	}

	glm::vec3 get_intersect() {
		return m_intersection;
	}
};

class Object {
	unsigned int m_VAO, m_VBO;
	unsigned int m_num_verts;
	bool m_rotate = false;

	Triangle m_polys[100];
	unsigned int m_num_polys;

	glm::mat4 m_model = glm::mat4(1.0f);

	glm::vec3 m_translation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);

	glm::vec3 m_color = glm::vec3(0.463f, 0.275f, 0.137f);

public:
	Object(float * verts, unsigned int num_verts) {
		m_num_verts = num_verts;

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		activate();

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, num_verts * 6 * sizeof(float), verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_num_polys = m_num_verts / 3;

		for (unsigned int i = 0; i < m_num_polys; ++i) {
			float* v_chunk = verts + i * 9;
			m_polys[i] = Triangle(v_chunk, &m_model);
		}

	}
	void activate() {
		glBindVertexArray(m_VAO);
	}
	void update() {
		m_model = glm::mat4(1.0f);

		m_model = glm::translate(m_model, m_translation);
		if (m_rotate) {
			m_model = glm::rotate(m_model, (float)glfwGetTime() / 2, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		//m_model = glm::rotate(m_model, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		m_model = glm::scale(m_model, m_scale);

	}
	void draw(Shader shader) {
		activate();
		shader.use();
		shader.setMat4("model", m_model);
		shader.setVec3("objectColor", m_color);
		glDrawArrays(GL_TRIANGLES, 0, m_num_verts);
	}
	bool intersect(glm::vec3 ray_pos, glm::vec3 ray_vector) {
		bool intersected = false;

		// iterates through all the triangles in the object
		for (unsigned int i = 0; i < m_num_polys; ++i) {
			if (m_polys[i].ray_intersection(ray_pos, ray_vector)) {
				intersected = true;
				break;
			}
		}
		return intersected;
	}
	void translate(glm::vec3 translate) {
		m_translation = translate;
	}
	void scale(glm::vec3 scale) {
		m_scale = scale;
	}
	void color(glm::vec3 color) {
		m_color = color;
	}
	void rotate(bool r) {
		m_rotate = r;
	}
	glm::mat4 get_model() {
		return m_model;
	}
};

class Line {
	unsigned int m_VAO, m_VBO;
	glm::mat4 m_model;

	glm::vec3 m_translation = glm::vec3(-1.0, -1.0, 1.0);

public:
	Line(float * verts) {
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		
		activate();

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, 24, verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	void activate() {
		glBindVertexArray(m_VAO);
	}
	void update() {
		m_model = glm::mat4(1.0f);

		m_model = glm::translate(m_model, m_translation);
		m_model = glm::scale(m_model, glm::vec3(3.0f));
	}
	void draw(Shader shader) {
		activate();
		shader.use();
		shader.setMat4("model", m_model);
		shader.setVec3("objectColor", glm::vec3(1.0, 1.0, 1.0));
		glDrawArrays(GL_LINES, 0, 2);
	}
	void translate(glm::vec3 translate) {
		m_translation = translate;
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader object_shader("vertex.glsl", "fragment.glsl");
	Shader lamp_shader("lamp_vertex.glsl", "lamp_fragment.glsl");

	float quad_verts[] = {
		0.0f, -1.0f, 1.0f,
		0.0f, 1.0f, -1.0f,
		0.0f, -1.0f, -1.0f,

		0.0f, -1.0f, 1.0f,
		0.0f, 1.0f, -1.0f,
		0.0f, 1.0f, 1.0f
	};

	glm::vec3 ray_vector = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 lamp_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lamp_pos = glm::vec3(1.0f, 1.0f, 1.0f);

	//								   fov					aspect ratio	  near   far
	projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

	//					    position				  target					up
	view = glm::lookAt(glm::vec3(5.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	Object quad(unit_cube, 36);
	Line ln(unit_line);

	unsigned int lampVAO, lampVBO;
	glGenVertexArrays(1, &lampVAO);
	glGenBuffers(1, &lampVBO);

	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unit_cube_no_normals), unit_cube_no_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	

	lamp_shader.use();

	lamp_shader.setMat4("projection", projection);
	lamp_shader.setMat4("view", view);

	
	object_shader.use();
	
	object_shader.setVec3("lampColor", lamp_color);
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

		processInput(window);

		glClearColor(background[0], background[1], background[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::vec3 ray_pos = get_mouse_loc(window, 3.0f);

		lamp_pos = ray_pos;

		lamp_shader.use();
		glm::mat4 lamp_model = glm::mat4(1.0f);
		lamp_model = glm::translate(lamp_model, lamp_pos);
		lamp_model = glm::scale(lamp_model, glm::vec3(0.25f));
		lamp_shader.setMat4("model", lamp_model);

		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		object_shader.use();
		object_shader.setVec3("lightPos", lamp_pos);


		quad.rotate(true);

		ln.translate(ray_pos);
		ln.update();
		ln.draw(lamp_shader);

		quad.update();
		quad.draw(object_shader);


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

void mouse_callback(GLFWwindow* window, int button, int action, int modifier) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	pause = (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS);

}

float mag(glm::vec3 vector) {
	// simple 3D pythagorean theorem calculator
	float a = vector[0];
	float b = vector[1];
	float c = vector[2];

	return pow(a * a + b * b + c * c, 0.5);
}
