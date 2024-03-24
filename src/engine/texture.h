#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class Texture2D {
private:
	GLuint m_textureID{};
	GLuint m_VAO{};

	int m_width{};
	int m_height{};
	int m_nrChannels{};

	bool m_VAOGenerated{ false };

public:
	Texture2D(const char* filepath);
	void createVAO() {
		float vertices[] = {
			// positions			// texture coords
			    0.0f, m_height, 0.0f,	1.0f, 0.0f, // top left
			 m_width, m_height, 0.0f,	0.0f, 0.0f, // top right
			    0.0f,     0.0f, 0.0f,	1.0f, 1.0f, // bottom left
		  	 m_width,     0.0f, 0.0f,	0.0f, 1.0f, // bottom right
		};

		unsigned int indices[] = {
			0, 1, 2,
			3, 2, 1
		};

		// vertex array object, vertex buffer object, Element Buffer Object
		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Connect everything to the VAO
		glBindVertexArray(VAO);

		// vertex data buffer, put data in a buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// define what points are used to make the shapes, reusing vertecies from VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		// define how the different attributes data are stored

		// vertex coords
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture mapping
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_VAOGenerated = true;
		m_VAO = VAO;
	};

	void draw() {
		if (!m_VAOGenerated) {
			createVAO();
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	};

	GLuint ID() { return m_textureID; }
	int width() { return m_width; }
	int height() { return m_height; }

};



#endif // OPENGL_TEXTURE_H
