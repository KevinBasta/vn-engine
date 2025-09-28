#ifndef VN_LAYER_TRANSITION
#define VN_LAYER_TRANSITION

#include "window.h"
#include "shader.h"
#include "state_subject.h"

class TransitionLayer {
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader* m_textureShader{ nullptr };

	// TODO: implement fade by having a black square with opacity, an auto action
	void drawTransition();

public:
	TransitionLayer(VnWindow* window, StateSubject* stateSubject, Shader* textureShader) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textureShader{ textureShader }
	{
	}

	void pollAndDraw() {

	};
};

#endif // VN_LAYER_TRANSITION