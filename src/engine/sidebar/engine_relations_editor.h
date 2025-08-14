#ifndef VN_ENGINE_RELATIONS_CONFIG_H
#define VN_ENGINE_RELATIONS_CONFIG_H

#include "state_subject.h"

class VnEngineRelationsEditor {
private:
	static StateSubject* m_stateSubject;

public:
	VnEngineRelationsEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineRelationsEditor() {}


public:
	void draw() {

	}

};

#endif // VN_ENGINE_RELATIONS_CONFIG_H
