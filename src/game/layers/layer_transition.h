#ifndef VN_LAYER_TRANSITION
#define VN_LAYER_TRANSITION

#include "window.h"
#include "shader.h"
#include "state_subject.h"

#define TEMP_VERTEX_PATH	VN_BASE_PATH"/src/game_internals/glsl/vertex_texture.glsl"
#define TEMP_FRAGMENT_PATH	VN_BASE_PATH"/src/game_internals/glsl/fragment_texture.glsl"

class TransitionLayer {
	VnWindow* m_window{ nullptr };
	Shader m_defaultShader;
	StateSubject* m_stateSubject{ nullptr };

	// TODO: implement fade by having a black square with opacity, an auto action
	void drawTransition();

public:
	TransitionLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH }
	{
	}

	void pollAndDraw() {

	};
};

#endif // VN_LAYER_TRANSITION