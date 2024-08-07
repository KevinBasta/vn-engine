#ifndef VN_STATE_RELATIONS_H
#define VN_STATE_RELATIONS_H

#include "model_subject.h"

class StateRelations {
public:
	std::vector<Relations> m_characterRelationsData{};

	void initCharacterRelations() {
		m_characterRelationsData = ModelSubject::getBaseRelations();
	}


public:
	//
	// Node interface
	//
	void handle(ActionRelationModify& action) {

	}
};


#endif // VN_STATE_RELATIONS_H
