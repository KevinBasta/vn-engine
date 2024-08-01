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

public:
	GameContext(VnWindow* window, StateSubject* stateSubject);
	
	void drawLayers() {
		
		m_backgroundLayer.pollAndDraw();
		
		m_spriteLayer.pollAndDraw();
		
		
		if (m_stateSubject->m_choices.isChoiceActive()) {
			m_choiceLayer.pollAndDraw();
		}
		else {
			m_textLayer.pollAndDraw();
		}

	};

	//void reloadShaders();
	/*if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		characterShader.reload();
		m_camera.resetCamera();
	}*/
};

#endif // VN_GAME_CONTEXT_H
