
#include "state_relations.h"

#include "id.h"
#include "state_subject.h"
#include "model_subject.h"

#include <unordered_map>
#include <memory>

Relations::RelationValue StateRelations::getRelationValue(RelationRequested& relation) {
	Relations::RelationValue relationValue{ 0 };

	auto iter{ m_characterRelationsData.find(relation.m_characterOneId) };
	if (iter != m_characterRelationsData.end()) {
		relationValue = iter->second.get()->getCharacterRelation(
			relation.m_characterTwoId,
			relation.m_relationTypeId
		);
	}

	return relationValue;
}

bool StateRelations::eval(Relations::RelationValue valueOne, Relations::RelationValue valueTwo, RelationComparisonOperator comparisonOperator) {
	switch (comparisonOperator)
	{
	case RelationComparisonOperator::LESS_THAN:
		return valueOne < valueTwo;
	case RelationComparisonOperator::LESS_THAN_OR_EQUAL:
		return valueOne <= valueTwo;
	case RelationComparisonOperator::GREATER_THAN:
		return valueOne > valueTwo;
	case RelationComparisonOperator::GREATER_THAN_OR_EQUAL:
		return valueOne >= valueTwo;
	case RelationComparisonOperator::EQUAL:
		return valueOne == valueTwo;
	default:
		return false;
	}
}

bool StateRelations::eval(RelationValueComparison& condition) {
	Relations::RelationValue relationValue{ getRelationValue(condition.m_relation) };

	return eval(relationValue, condition.m_valueToCompare, condition.m_comparisonOperator);
}

bool StateRelations::eval(RelationRelationComparison& condition) {
	Relations::RelationValue relationOneValue{ getRelationValue(condition.m_relationOne) };
	Relations::RelationValue relationTwoValue{ getRelationValue(condition.m_relationTwo) };

	return eval(relationOneValue, relationTwoValue, condition.m_comparisonOperator);
}

bool StateRelations::eval(RelationConditionUnit& conditionUnit) {
	switch (conditionUnit.m_operator)
	{
	case RelationGroupingOperator::NONE:
	{
		return true;
	}
	case RelationGroupingOperator::AND:
	{
		for (auto& condition : conditionUnit.m_relationValueConditions) {
			if (!eval(condition)) {
				return false;
			}
		}

		for (auto& condition : conditionUnit.m_relationRelationConditions) {
			if (!eval(condition)) {
				return false;
			}
		}

		for (auto& unit : conditionUnit.m_conditionUnits) {
			if (!eval(unit)) {
				return false;
			}
		}

		return true;
	}
	case RelationGroupingOperator::OR:
	{
		for (auto& condition : conditionUnit.m_relationValueConditions) {
			if (eval(condition)) {
				return true;
			}
		}

		for (auto& condition : conditionUnit.m_relationRelationConditions) {
			if (eval(condition)) {
				return true;
			}
		}

		for (auto& unit : conditionUnit.m_conditionUnits) {
			if (eval(unit)) {
				return true;
			}
		}

		return false;
	}
	default:
		break;
	}

	return false;
}

void StateRelations::handle(const ActionRelationSetNextNode& action) {
	if (m_stateSubject == nullptr) { return; }

	for (auto condition : action.m_conditions) {
		bool result = eval(condition);

		if (result) {
			m_stateSubject->m_nextNode.set(action.m_nodeId);
			break;
		}
	}
}

void StateRelations::handle(const ActionRelationSetNextChapter& action) {
	if (m_stateSubject == nullptr) { return; }
	
	for (auto condition : action.m_conditions) {
		bool result = eval(condition);

		if (result) {
			m_stateSubject->m_nextChapter.set(action.m_chapterId);
			break;
		}
	}
}

void StateRelations::handle(const ActionRelationModify& action) {
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
