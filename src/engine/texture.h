#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

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

	bool m_VAOGenerated{ false };

private:
	void createVAO();
	void centerToScreen();

public:
	Texture2D(const char* filepath, TextureType type);

	void draw();

	GLuint ID()  const { return m_textureID; }
	int width()  const { return m_width; }
	int height() const { return m_height; }

};



#endif // OPENGL_TEXTURE_H
