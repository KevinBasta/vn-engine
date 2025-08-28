#ifndef VN_MAIN_MENU_LAYER_H
#define VN_MAIN_MENU_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"

#define TEMP_VERTEX_PATH	VN_BASE_PATH"/src/game_internals/glsl/vertex_texture.glsl"
#define TEMP_FRAGMENT_PATH	VN_BASE_PATH"/src/game_internals/glsl/fragment_texture.glsl"

#define TEMP_TEXT_VERTEX_SHADER		VN_BASE_PATH"/src/game_internals/glsl/vertex_text.glsl"
#define TEMP_TEXT_FRAGMENT_SHADER	VN_BASE_PATH"/src/game_internals/glsl/fragment_text.glsl"

class MainMenuLayer {
private:
	VnWindow* m_window{ nullptr };
	Shader m_defaultShader;
	Shader m_textShader;
	StateSubject* m_stateSubject{ nullptr };

	void drawBackground(const FrameDimensions& frame, TextureIdentifier texture);
	void drawOptions(const FrameDimensions& frame);

public:
	MainMenuLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH },
		m_textShader{ TEMP_TEXT_VERTEX_SHADER, TEMP_TEXT_FRAGMENT_SHADER }
	{
	}

	void pollAndDraw(const FrameDimensions& frame);
};


#endif // VN_MAIN_MENU_LAYER_H