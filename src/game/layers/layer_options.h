#ifndef VN_LAYER_OPTIONS_H
#define VN_LAYER_OPTIONS_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"

class OptionsMenuLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader* m_textShader{ nullptr };

	void drawOptions(const FrameDimensions& frame) {

	}

public:
	OptionsMenuLayer(VnWindow* window, StateSubject* stateSubject, Shader* textShader) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textShader{ textShader }
	{
	}

	void pollAndDraw(const FrameDimensions& frame) {

	}
};

#endif // VN_LAYER_OPTIONS_H