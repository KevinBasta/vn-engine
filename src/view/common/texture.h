#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include <iostream>

enum class TextureType {
	CHARACTER,
	BACKGROUND
};

class Texture2D {
private:
	GLuint m_textureID{};
	GLuint m_VAO{};

	int m_width{};
	int m_height{};
	int m_nrChannels{};

	float m_defaultScaleFactor{ 1.0f };
	glm::mat4 m_defaultModel{ glm::mat4(1.0f) };

	bool m_modelMatrixComputed{ false };
	bool m_VAOGenerated{ false };

private:
	void createVAO();
	void centerToScreen(float frameWidth, float frameHeight);

public:
	Texture2D(const char* filepath, TextureType type);

	void draw();

	// DeleteVAO();

	GLuint ID()  const { return m_textureID; }
	int width()  const { return m_width; }
	int height() const { return m_height; }
};



#endif // OPENGL_TEXTURE_H
