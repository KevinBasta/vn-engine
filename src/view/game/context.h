#ifndef VN_GAME_CONTEXT_H
#define VN_GAME_CONTEXT_H

#include "window.h"
#include "state_subject.h"
#include "texture.h"
#include "shader.h"

#include "layer_background.h"
#include "layer_sprite.h"
#include "layer_text.h"
#include "layer_choice.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GameContext {
public:
	VnWindow* m_window{};
	StateSubject* m_stateSubject{};
	BackgroundLayer m_backgroundLayer;
	SpriteLayer	m_spriteLayer;
	TextLayer m_textLayer;
	ChoiceLayer m_choiceLayer;
	Shader m_defaultShader;
	Shader m_screenShader;
	unsigned int m_quadVAO, m_quadVBO, m_framebuffer, m_textureColorbuffer, m_rbo;

	void drawLayers(const FrameDimentions frameDimentions) {
		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glEnable(GL_DEPTH_TEST);

		// clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw each layer
		m_backgroundLayer.pollAndDraw(frameDimentions);
		m_spriteLayer.pollAndDraw(frameDimentions);
		
		if (m_stateSubject->m_choices.isChoiceActive()) {
			m_choiceLayer.pollAndDraw();
		}
		else {
			m_textLayer.pollAndDraw(frameDimentions);
		}

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

public:
	GameContext(VnWindow* window, StateSubject* stateSubject);
	

	void renderEngine(const FrameDimentions frameDimentions) {
		rescaleFramebuffer(frameDimentions);
		drawLayers(frameDimentions);

		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		m_screenShader.use();
		glBindVertexArray(m_quadVAO);
		glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderRuntime() {
		rescaleFramebuffer(m_window->getFrameDimentions());
		drawLayers(m_window->getFrameDimentions());
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_screenShader.use();
		glBindVertexArray(m_quadVAO);
		glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	unsigned int getTextureId() {
		return m_textureColorbuffer;
	}

	void rescaleFramebuffer(const FrameDimentions frameDimentions) {
		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameDimentions.frameWidth, frameDimentions.frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameDimentions.frameWidth, frameDimentions.frameHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	// TODO: dealloc all OpenGL buffers, can't be done on destruction because opengl might have been uninit by that point?
	/*void uninit() {
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &planeVAO);
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &cubeVBO);
		glDeleteBuffers(1, &planeVBO);
		glDeleteBuffers(1, &quadVBO);
		glDeleteRenderbuffers(1, &rbo);
		glDeleteFramebuffers(1, &framebuffer);
	}*/

	//void reloadShaders();
	/*if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		characterShader.reload();
		m_camera.resetCamera();
	}*/
};

#endif // VN_GAME_CONTEXT_H
