
#include "shader.h"
#include "context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <utility>

#include "model_subject.h"

#define TEMP_FRAMEBUFFER_VERTEX		VN_BASE_PATH"/src/game_internals/glsl/vertex_framebuffer.glsl"
#define TEMP_FRAMEBUFFER_FRAGMENT	VN_BASE_PATH"/src/game_internals/glsl/fragment_framebuffer.glsl"

// TODO: init both shaders here first then pass to the layers
// TODO: put camera object in context to contain ortho?
// TODO: change the shader interfact to allow better uniform passing,
// maybe registering the uniform names and a function to set by name/enum type
GameContext::GameContext(VnWindow* window, StateSubject* stateSubject) :
	m_window{ window },
	m_stateSubject{ stateSubject },
	m_backgroundLayer{ window, stateSubject },
	m_spriteLayer{ window, stateSubject },
	m_textLayer{ window, stateSubject },
	m_choiceLayer{ window, stateSubject },
	m_mainMenuLayer{ window, stateSubject },
	m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH },
	m_screenShader{	TEMP_FRAMEBUFFER_VERTEX, TEMP_FRAMEBUFFER_FRAGMENT }

{
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// create a color attachment texture
	glGenTextures(1, &m_textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_window->width(), m_window->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_window->width(), m_window->height()); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAMEBUFFER NOT COMPLETE, ERROR!!!" << std::endl;
		// TODO: send error to user error log
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Uncomment to enable transparent backgrounds
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}




