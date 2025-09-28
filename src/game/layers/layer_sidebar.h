#ifndef VN_LAYER_SIDEBAR_H
#define VN_LAYER_SIDEBAR_H

#include "window.h"
#include "shader.h"
#include "state_subject.h"

class SideBarLayer {
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader* m_textureShader{ nullptr };

public:
	SideBarLayer(VnWindow* window, StateSubject* stateSubject, Shader* textureShader) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textureShader{ textureShader }
	{
	}

	void pollAndDraw() {

	};
};

#endif // VN_LAYER_SIDEBAR_H