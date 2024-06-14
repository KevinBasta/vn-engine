
#include <filesystem> 

#include "texture.h"
#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <utility>

Texture2D::Texture2D(const char* filepath) {
	if (filepath == nullptr) {
		return;
	}

	createTexture(filepath);
	createVAO();
}

Texture2D::~Texture2D() {
	std::cout << "deleting texture" << std::endl;
	deleteTexture();
	deleteVAO();
}

void Texture2D::draw() {
	if (!m_generatedVAO || !m_generatedTexture) {
		std::cout << "texture not generated" << std::endl;
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// unbind texture???
};

void Texture2D::createTexture(const char* filepath) {
	std::filesystem::path path{ filepath };

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// TODO: Unsure if this needs to be here or when drawing or program level

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

	m_generatedTexture = true;

	// unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::deleteTexture() {
	glDeleteTextures(1, &m_textureID);
}


void Texture2D::createVAO() {
	float rightX{ static_cast<float>(m_width) };
	float leftX{ 0.0f };

	float topY{ static_cast<float>(m_height) };
	float bottomY{ 0.0f };

	float vertices[] = {
		// positions			// texture coords
		   leftX,    topY, 0.0f,	1.0f, 0.0f, // top left
		  rightX,    topY, 0.0f,	0.0f, 0.0f, // top right
		   leftX, bottomY, 0.0f,	1.0f, 1.0f, // bottom left
		  rightX, bottomY, 0.0f,	0.0f, 1.0f, // bottom right
	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 2, 1
	};

	// vertex array object, vertex buffer object, Element Buffer Object
	GLuint VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// connect everything to the VAO
	glBindVertexArray(m_VAO);

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

	m_generatedVAO = true;

	// unbind all buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

void Texture2D::deleteVAO() {
	glDeleteVertexArrays(1, &m_VAO);
}

float Texture2D::getScaleToViewport(VnWindow* window) {
	if (window == nullptr) {
		// TODO: raise exception?
		return 1.0f;
	}

	float scaleFactor	{ 0.0f };

	float frameWidth	{ static_cast<float>(window->width()) };
	float frameHeight	{ static_cast<float>(window->height()) };

	float imageWidth	{ static_cast<float>(m_width) };
	float imageHeight	{ static_cast<float>(m_height) };

	//std::cout << "Image width" << imageWidth << std::endl;
	//std::cout << "frame width" << frameWidth << std::endl;
	//std::cout << "Image height" << imageHeight << std::endl;
	//std::cout << "frame height" << frameHeight << std::endl;

	// TODO: may still be incorrect, background squished
	if (imageWidth > imageHeight) {
		scaleFactor = std::max(imageWidth, frameWidth) / std::min(imageWidth, frameWidth);
	}
	else {
		scaleFactor = std::max(imageHeight, frameHeight) / std::min(imageHeight, frameHeight);
	}


	if (imageWidth > frameWidth || imageHeight > frameHeight) {
		scaleFactor = 1 / scaleFactor;
	}
	
	return scaleFactor;
}
