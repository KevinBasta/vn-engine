#ifndef VN_LAYER_SAVES_H
#define VN_LAYER_SAVES_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"

class SavesMenuLayer {
public:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };

	SavesMenuLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject }
	{

	}
	void pollAndDraw(const FrameDimensions& frame) {

	}
};

#endif // VN_LAYER_SAVES_H