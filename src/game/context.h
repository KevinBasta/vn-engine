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
#include "layer_main_menu.h"
#include "layer_saves.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define DBG_VERTEX_PATH	VN_BASE_PATH"/src/game_internals/glsl/vertex_debug.glsl"
#define DBG_FRAGMENT_PATH	VN_BASE_PATH"/src/game_internals/glsl/fragment_debug.glsl"

class GameContext {
public:
	VnWindow* m_window{};
	StateSubject* m_stateSubject{};
	BackgroundLayer m_backgroundLayer;
	SpriteLayer	m_spriteLayer;
	TextLayer m_textLayer;
	ChoiceLayer m_choiceLayer;
	MainMenuLayer m_mainMenuLayer;
	SavesMenuLayer m_savesMenuLayer;
	Shader m_defaultShader;
	Shader m_screenShader;
	unsigned int m_quadVAO, m_quadVBO, m_framebuffer, m_textureColorbuffer, m_rbo;

	void drawLayers(const FrameDimensions& frame) {
		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glEnable(GL_DEPTH_TEST);

		// Clear the framebuffer's content
		glClearColor(0.5f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (VNFSA::inMainMenu()) {
			m_mainMenuLayer.pollAndDraw(frame);
		}
		else if (VNFSA::inSavesMenu()) {
			m_savesMenuLayer.pollAndDraw(frame);
		}
		else if (VNFSA::inGame()) {
			// Draw each layer
			m_backgroundLayer.pollAndDraw(frame);
			m_spriteLayer.pollAndDraw(frame);
		
			if (m_stateSubject->m_choices.isChoiceActive()) {
				m_choiceLayer.pollAndDraw(frame);
			}
			else {
				m_textLayer.pollAndDraw(frame);
			}
		
			if (VNFSA::optionsSidebarOpen()) {

			}
		}

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

public:
	GameContext(VnWindow* window, StateSubject* stateSubject);
	
	void renderEngine(const FrameDimensions& frame) {
		rescaleFramebuffer(frame);
		drawLayers(frame);

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
		FrameDimensions frame{ m_window->getFrameDimensions() };

		// Must set viewport to normal viewport before creating framebuffer
		// otherwise the framebuffer will be created for the current viewport xy offset
		glViewport(0, 0, frame.width, frame.height);

		rescaleFramebuffer(frame);
		drawLayers(frame);
		
		// Set the viewport back to it's window centered position
		glViewport(frame.x, frame.y, frame.width, frame.height);



		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		m_screenShader.use();
		glBindVertexArray(m_quadVAO);
		glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	unsigned int getTextureId() {
		return m_textureColorbuffer;
	}

	void rescaleFramebuffer(const FrameDimensions& frame) {
		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.width, frame.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frame.width, frame.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
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

private:
	// Debug function
	void debugTriangle() {
		float vertices[] = {
			-1.0f, -1.0f, 0.0f, // left  
			 0.5f, -0.5f, 0.0f, // right 
			 0.0f,  0.5f, 0.0f  // top   
		};

		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
		Shader test{ DBG_VERTEX_PATH, DBG_FRAGMENT_PATH };

		test.use();
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};

#endif // VN_GAME_CONTEXT_H
