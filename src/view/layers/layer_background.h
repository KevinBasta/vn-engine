#ifndef VN_BACKGROUND_LAYER_H
#define VN_BACKGROUND_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_subject.h"

#define TEMP_VERTEX_PATH	BASE_PATH"\\src\\view\\glsl\\defaultVertex.glsl"
#define TEMP_FRAGMENT_PATH	BASE_PATH"\\src\\view\\glsl\\defaultFragment.glsl"

class BackgroundLayer {
private:
	VnWindow* m_window{ nullptr };
	Shader m_defaultShader;
	StateSubject* m_stateSubject{ nullptr };

	void drawBackground(Texture2D* texture);

public:
	BackgroundLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH }
	{
	}

	void pollAndDraw();
};



#endif // VN_BACKGROUND_LAYER_H
