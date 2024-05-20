
#include <filesystem> 

#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <utility>

Texture2D::Texture2D(const char* filepath, TextureType type) {
	std::filesystem::path path{ filepath };

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and geneate the texture
	unsigned char* data = stbi_load(filepath, &m_width, &m_height, &m_nrChannels, 0);

	// load with RGB or RGBA depending on extention
	if (data) {
		if (path.extension() == ".png") {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			std::cout << filepath << "used png" << std::endl;
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			std::cout << filepath << "used jpg" << std::endl;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	switch (type)
	{
	case TextureType::CHARACTER:
		
		break;
	case TextureType::BACKGROUND:
		
		break;
	default:
		break;
	}
}

void Texture2D::draw() {
	if (!m_VAOGenerated) {
		createVAO();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};


void Texture2D::createVAO() {
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

void Texture2D::centerToScreen(float frameWidth, float frameHeight) {
	float imageWidth	{ static_cast<float>(m_width) };
	float imageHeight	{ static_cast<float>(m_height) };

	if (imageWidth > imageHeight) {
		m_defaultScaleFactor = 1 / (std::max(imageWidth, frameWidth) - std::min(imageWidth, frameWidth));
	}
	else {
		m_defaultScaleFactor = 1 / (std::max(imageHeight, frameHeight) - std::min(imageHeight, frameHeight));
	}

	m_defaultModel = glm::scale(m_defaultModel, glm::vec3(m_defaultScaleFactor, m_defaultScaleFactor, 0.0f));
	m_defaultModel = glm::translate(m_defaultModel, glm::vec3(-1 * (imageWidth / 2), -1 * (imageHeight / 2), -1.0f));
}