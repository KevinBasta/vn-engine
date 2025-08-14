#ifndef VN_ENGINE_TEXTURES_CONFIG_H
#define VN_ENGINE_TEXTURES_CONFIG_H

#include "state_subject.h"

class VnEngineTexturesEditor {
private:
	static StateSubject* m_stateSubject;

public:
	VnEngineTexturesEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineTexturesEditor() {}


public:
	void draw() {

	}

};

#endif // VN_ENGINE_TEXTURES_CONFIG_H
