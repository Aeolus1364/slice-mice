#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"

class Lamp {
	unsigned int m_VAO, m_VBO;
	unsigned int m_num_verts = 36;

	float m_verts[108] = {
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

	glm::mat4 m_model = glm::mat4(1.0f);

	glm::vec3 m_translation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(0.25f);

	glm::vec3 m_color = glm::vec3(1.0f, 1.0f, 1.0f);

public:
	Lamp() {
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		activate();

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_num_verts * 6 * sizeof(float), m_verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}
	Lamp(glm::vec3 color) {
		Lamp();
		m_color = color;
	}
	void activate() {
		glBindVertexArray(m_VAO);
	}
	void update() {
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, m_translation);
		m_model = glm::scale(m_model, m_scale);
	}
	void draw(Shader shader) {
		activate();
		shader.use();
		shader.setMat4("model", m_model);
		shader.setVec3("color", m_color);
		glDrawArrays(GL_TRIANGLES, 0, m_num_verts);
	}
	void translate(glm::vec3 translate) {
		m_translation = translate;
	}
	glm::vec3 get_color() {
		return m_color;
	}
};

class Triangle {
	glm::vec4 m_verts[3];
	glm::mat4* m_model;
	glm::vec3 m_intersection;

	glm::vec3 a_world, b_world, c_world;
	glm::vec3 ab, ca, bc;
	glm::vec3 m_norm;

public:
	Triangle() {

	}
	Triangle(float* vertices, glm::mat4* model) {
		m_verts[0] = glm::vec4(vertices[0], vertices[1], vertices[2], 0.0f);
		m_verts[1] = glm::vec4(vertices[6], vertices[7], vertices[8], 0.0f);
		m_verts[2] = glm::vec4(vertices[12], vertices[13], vertices[14], 0.0f);

		/*std::cout << m_verts[0][0] << " " << m_verts[0][1] << " " << m_verts[0][2] << std::endl;
		std::cout << m_verts[1][0] << " " << m_verts[1][1] << " " << m_verts[1][2] << std::endl;
		std::cout << m_verts[2][0] << " " << m_verts[2][1] << " " << m_verts[2][2] << std::endl;*/

		m_model = model;
		m_intersection = glm::vec3(0.0f);
	}
	void update() {
		// converting from local coordinates to world coordinates
		a_world = *m_model * m_verts[0];
		b_world = *m_model * m_verts[1];
		c_world = *m_model * m_verts[2];

		// calculates the vectors for each side of the triangle
		ab = b_world - a_world;
		ca = a_world - c_world;
		bc = c_world - b_world;

		// normal vector calculated by crossing two triangle vectors
		m_norm = glm::cross(ca, ab);
	}
	bool plane_intersection(glm::vec3 p_pos, glm::vec3 p_norm) {
		update();

		glm::vec3 ln_vec = glm::cross(p_norm, m_norm);
		
		float ax, ay;

		if (ln_vec[0] != 0) {
			ax = ln_vec[1];
			ay = ln_vec[2];
		} else if (ln_vec[1] != 0) {
			ax = ln_vec[0];
			ay = ln_vec[2];
		} else if (ln_vec[2] != 0) {
			ax = ln_vec[0];
			ay = ln_vec[1];
		}

		std::cout << ln_vec[0] << " " << ln_vec[1] << " " << ln_vec[2] << std::endl;
		std::cout << ax << " " << ay << std::endl;


		return true;
	}
	bool ray_intersection(glm::vec3 ray_pos, glm::vec3 ray_vector) {
		// updates a bunch of things to account for change in model vector since construction
		update();

		// w is the vector from the ray's to the point defining the plane
		glm::vec3 w = a_world - ray_pos;

		// calculates the ray's intersection with the plane
		glm::vec3 intersect = ray_pos + (glm::dot(w, m_norm) / glm::dot(ray_vector, m_norm)) * ray_vector;

		// calculates vectors pointing from the intersection point to each point on the triangle
		glm::vec3 ia = intersect - a_world;
		glm::vec3 ib = intersect - b_world;
		glm::vec3 ic = intersect - c_world;

		// calculates 3 values that are positive or negative based on which side of each line the point is on
		float side_ab = glm::dot(glm::cross(ab, ia), m_norm);
		float side_bc = glm::dot(glm::cross(bc, ib), m_norm);
		float side_ca = glm::dot(glm::cross(ca, ic), m_norm);

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
	Object(float* verts, unsigned int num_verts) {
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
			float* v_chunk = verts + i * 18;
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
		m_model = glm::scale(m_model, m_scale);
	}
	void draw(Shader shader) {
		activate();
		shader.use();
		shader.setMat4("model", m_model);
		shader.setVec3("objectColor", m_color);
		glDrawArrays(GL_TRIANGLES, 0, m_num_verts);
	}
	bool line_intersect(glm::vec3 ray_pos, glm::vec3 ray_vector) {
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
	void plane_intersect(glm::vec3 p_pos, glm::vec3 p_norm) {
		for (unsigned int i = 0; i < m_num_polys; i++) {
			if (m_polys[i].plane_intersection(p_pos, p_norm)) {

			}
		}
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
	unsigned int m_line_VAO, m_line_VBO;
	unsigned int m_plane_VAO, m_plane_VBO;
	glm::mat4 m_model = glm::mat4(1.0f);
	glm::vec3 m_color = glm::vec3(1.0f);

	glm::vec3 ray_vector = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 current_pos;
	glm::vec3 pinned_pos;

	bool pinned = false;
	bool cut_ready = false;

	// quad rendering stuff
	glm::vec3 coords[4];
	float verts[18];

	// plane data
	glm::vec3 p_normal;
	glm::vec3 p_point;

public:
	Line(float* verts) {
		// line set up
		glGenVertexArrays(1, &m_line_VAO);
		glGenBuffers(1, &m_line_VBO);

		glBindVertexArray(m_line_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_line_VBO);
		glBufferData(GL_ARRAY_BUFFER, 24, verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// plane set up
		glGenVertexArrays(1, &m_plane_VAO);
		glGenBuffers(1, &m_plane_VBO);

		glBindVertexArray(m_plane_VAO);

		//gen_quad();

		glBindBuffer(GL_ARRAY_BUFFER, m_plane_VBO);
		glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), verts, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	void update() {
		m_model = glm::mat4(1.0f);

		m_model = glm::translate(m_model, current_pos);
		m_model = glm::scale(m_model, glm::vec3(4.0f));
	}
	void draw(Shader shader) {
		shader.use();
		shader.setMat4("model", m_model);
		shader.setVec3("color", m_color);
		// drawing line
		glBindVertexArray(m_line_VAO);
		glDrawArrays(GL_LINES, 0, 2);

		// drawing plane
		if (pinned) {
			// removes transformations for plane rendering
			shader.setMat4("model", glm::mat4(1.0f));
			// updates quad 
			gen_quad();
			glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), verts, GL_DYNAMIC_DRAW);
			glBindVertexArray(m_plane_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	void update_pos(glm::vec3 translate) {
		current_pos = translate;
	}
	void set_color(glm::vec3 color) {
		m_color = color;
	}
	void gen_quad() {
		// generates coords for pinned side of plane
		coords[0] = pinned_pos;
		coords[1] = pinned_pos + 4.0f * ray_vector;
		// generates coords for active side of plane
		coords[2] = current_pos;
		coords[3] = current_pos + 4.0f * ray_vector;

		// converts coords into float array for rendering
		verts[0]  = coords[0][0]; verts[1]  = coords[0][1]; verts[2]  = coords[0][2];
		verts[3]  = coords[1][0]; verts[4]  = coords[1][1]; verts[5]  = coords[1][2];
		verts[6]  = coords[2][0]; verts[7]  = coords[2][1]; verts[8]  = coords[2][2];
		verts[9]  = coords[1][0]; verts[10] = coords[1][1]; verts[11] = coords[1][2];
		verts[12] = coords[2][0]; verts[13] = coords[2][1]; verts[14] = coords[2][2];
		verts[15] = coords[3][0]; verts[16] = coords[3][1]; verts[17] = coords[3][2];
	}
	void gen_plane() {
		// setting plane vector and point
		p_normal = glm::cross(pinned_pos - current_pos, ray_vector);
		p_point = pinned_pos;
		cut_ready = true;
	}
	std::pair<glm::vec3, glm::vec3> cut_plane() {
		cut_ready = false;
		return std::make_pair(p_normal, p_point);
	}
	bool is_cut_ready() {
		return cut_ready;
	}
	void pin() {
		pinned = true;
		pinned_pos = current_pos;
	}
	void unpin(bool succeed) {
		// only generates cutting plane if the unpin succeeds AND it was previously pinned (didn't start in the object)
		if (succeed && pinned) {
			gen_plane();
		}
		pinned = false;
	}
};