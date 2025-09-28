
#include "id.h"
#include "relations.h"
#include "node_types.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

int Relations::getRelationWith(int characterId, int relationId) {
	CharacterToRelationMap::iterator relationWithCharacter{ m_relationships.find(characterId) };

	if (relationWithCharacter == m_relationships.end()) {
		return 0;
	}

	RelationToValueMap::iterator relationValue{ relationWithCharacter->second.find(relationId) };

	if (relationValue == relationWithCharacter->second.end()) {
		return 0;
	}

	return relationWithCharacter->second[relationId];
}

bool Relations::modifyRelationWith(int characterId, int relationId, RelationModification modification, int amount) {
	bool operationDone{ false };
	
	switch (modification)
	{
	case RelationModification::SET:
	{
		m_relationships[characterId][relationId] = amount;
		operationDone = true;
		break;
	}
	case RelationModification::ADD:
	{
		m_relationships[characterId][relationId] += amount;
		operationDone = true;
		break;
	}
	case RelationModification::SUBTRACT:
	{
		m_relationships[characterId][relationId] -= amount;
		operationDone = true;
		break;
	}
	case RelationModification::MULTIPLY:
	{
		m_relationships[characterId][relationId] *= amount;
		operationDone = true;
		break;
	}
	case RelationModification::DIVIDE:
	{
		if (amount != 0) {
			m_relationships[characterId][relationId] /= amount;
			operationDone = true;
		}
		break;
	}
	default:
		break;
	}

	return operationDone;
}

std::ostream& operator<<(std::ostream& out, Relations& relation) {
	out << "Relations:" << std::endl;

	for (auto characterRelations : relation.m_relationships) {
		out << "\tcharacter id: " << characterRelations.first << std::endl;

		for (auto relationValues : characterRelations.second) {
			out << "\t\trelation id: " << relationValues.first << " relation value: " << relationValues.second << std::endl;
		}
	}

	return out;
}
