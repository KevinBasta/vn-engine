#ifndef VN_LAYER_OPTIONS_H
#define VN_LAYER_OPTIONS_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"

#define TEMP_TEXT_VERTEX_SHADER		VN_BASE_PATH"/src/game_internals/glsl/vertex_text.glsl"
#define TEMP_TEXT_FRAGMENT_SHADER	VN_BASE_PATH"/src/game_internals/glsl/fragment_text.glsl"

class OptionsMenuLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader m_textShader;

	void drawOptions(const FrameDimensions& frame) {

	}

public:
	OptionsMenuLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textShader{ TEMP_TEXT_VERTEX_SHADER, TEMP_TEXT_FRAGMENT_SHADER }
	{
	}

	void pollAndDraw(const FrameDimensions& frame) {

	}
};

#endif // VN_LAYER_OPTIONS_H