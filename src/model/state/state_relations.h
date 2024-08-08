#ifndef VN_STATE_RELATIONS_H
#define VN_STATE_RELATIONS_H

#include "id.h"
#include "model_subject.h"

#include <unordered_map>
#include <memory>

class StateRelations {
private:
	StateSubject* m_stateSubject{ nullptr };
	std::unordered_map<id, std::unique_ptr<Relations>> m_characterRelationsData{};

	bool eval(ActionRelationConditionUnit& conditionUnit) {

		return false;
	}

	void print() {
		for (auto& [characterId, relationUniquePtr] : m_characterRelationsData) {
			std::cout << *relationUniquePtr.get();
		}
	}

public:
	StateRelations(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}

	void reset() {
		m_characterRelationsData.clear();

		for (const auto& [characterId, baseRelationUniquePtr] : ModelSubject::getBaseRelations()) {
			Relations* const baseRelationsObj{ baseRelationUniquePtr.get() };
			Relations* newRelationObj{ new Relations{*baseRelationsObj} };

			m_characterRelationsData[characterId] = std::unique_ptr<Relations>{ newRelationObj };
		}
	}

public:
	void save() {}
	void load() {}

public:
	//
	// Node interface
	//
	void handle(ActionRelationModify& action) {
		if (m_characterRelationsData[action.m_relation.m_characterOneId].get() == nullptr) {
			m_characterRelationsData[action.m_relation.m_characterOneId] = std::make_unique<Relations>();
		}

		m_characterRelationsData[action.m_relation.m_characterOneId].get()->modifyCharacterRelation(
			action.m_relation.m_characterTwoId, 
			action.m_relation.m_relationTypeId, 
			action.m_modificationType, 
			action.m_modificationValue
		);
		
		print();
	}

	void handle(ActionRelationSetNextNode& action);
};


#endif // VN_STATE_RELATIONS_H
