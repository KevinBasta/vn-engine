#ifndef VN_MAIN_MENU_LAYER_H
#define VN_MAIN_MENU_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"

class MainMenuLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader* m_textShader{ nullptr };

	void drawOptions(const FrameDimensions& frame);

public:
	MainMenuLayer(VnWindow* window, StateSubject* stateSubject, Shader* textShader) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textShader{ textShader }
	{
	}

	void pollAndDraw(const FrameDimensions& frame);
};


#endif // VN_MAIN_MENU_LAYER_H