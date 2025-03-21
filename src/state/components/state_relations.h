#ifndef VN_STATE_RELATIONS_H
#define VN_STATE_RELATIONS_H

#include "id.h"
#include "relations.h"
#include "model_subject.h"
#include "model_runtime_interface.h"

#include <unordered_map>
#include <memory>

class StateRelations {
private:
	StateSubject* m_stateSubject{ nullptr };
	std::unordered_map<id, std::unique_ptr<Relations>> m_characterRelationsData{};

	Relations::RelationValue getRelationValue(CharacterRelation& relation);
	bool eval(Relations::RelationValue valueOne, Relations::RelationValue valueTwo, RelationComparisonOperator comparisonOperator);
	bool eval(RelationValueComparison& condition);
	bool eval(RelationRelationComparison& condition);
	bool eval(RelationConditionUnit& conditionUnit);

	void print() {
		/*for (auto& [characterId, relationUniquePtr] : m_characterRelationsData) {
			std::cout << *relationUniquePtr.get();
		}*/
	}

public:
	StateRelations(StateSubject* stateSubject) : 
		m_stateSubject{ stateSubject } 
	{
	
	}

	void reset() {
		m_characterRelationsData.clear();

		for (const auto& [characterId, baseRelationUniquePtr] : ModelRuntimeInterface::getBaseRelations()) {
			Relations* const baseRelationsObj{ baseRelationUniquePtr.get() };
			Relations* newRelationObj{ new Relations{*baseRelationsObj} };

			m_characterRelationsData[characterId] = std::unique_ptr<Relations>{ newRelationObj };
		}
	}

public:
	//
	// Node interface
	//
	void handle(const ActionRelationModify& action);

	void handle(const ActionRelationSetNextNode& action);
	void handle(const ActionRelationSetNextChapter& action);

public:
	void save() {}
	void load() {}
};


#endif // VN_STATE_RELATIONS_H
