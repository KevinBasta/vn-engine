#ifndef VN_ENGINE_RELATIONS_CONFIG_H
#define VN_ENGINE_RELATIONS_CONFIG_H

#include "state_subject.h"

class VnEngineRelationEditor {
private:
	static StateSubject* m_stateSubject;

public:
	VnEngineRelationEditor(StateSubject* stateSubject)
	{
		m_stateSubject = stateSubject;
	}

	~VnEngineRelationEditor() {}


public:
	void draw() {

	}

};

#endif // VN_ENGINE_RELATIONS_CONFIG_H
