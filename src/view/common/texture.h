#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include <iostream>

class Texture2D {
private:
	// Opengl specific IDs
	GLuint m_textureID{};
	GLuint m_VAO{};

private:
	int m_width{};
	int m_height{};
	int m_nrChannels{};

	float m_defaultScaleFactor{ 1.0f };
	glm::mat4 m_defaultModel{ glm::mat4(1.0f) };

	bool m_modelMatrixComputed{ false };
	bool m_generatedTexture{ false };
	bool m_generatedVAO{ false };

private:
	void createTexture(const char* filepath);
	void deleteTexture();
	
	void createVAO();
	void deleteVAO();

public:
	Texture2D(const char* filepath);
	~Texture2D();
	void draw();

	GLuint ID()  const { return m_textureID; }
	int width()  const { return m_width; }
	int height() const { return m_height; }
	float getScaleToViewport();
};



#endif // OPENGL_TEXTURE_H
