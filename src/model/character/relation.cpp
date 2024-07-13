
#include "id.h"
#include "relation.h"
#include "relation_types.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

bool Relations::addRelationship(int characterId) {
	auto [relationshipEntry, result] = m_relationships.insert(std::make_pair(characterId, RelationToValueMap{}));

	return result;
}

int Relations::getCharacterRelation(int characterId, int relationId) {
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

bool Relations::setCharacterRelation(int characterId, int relationId, int amount) {
	CharacterToRelationMap::iterator relationWithCharacter{ m_relationships.find(characterId) };

	if (relationWithCharacter == m_relationships.end()) {
		bool relationshipInit{ addRelationship(characterId) };

		if (!relationshipInit) {
			return false;
		}
	}
	
	relationWithCharacter->second[relationId] = amount;
	return true;
}

bool Relations::addCharacterRelation(int characterId, int relationId, int amountToAdd) {
	CharacterToRelationMap::iterator relationWithCharacter{ m_relationships.find(characterId) };
	
	if (relationWithCharacter == m_relationships.end()) {
		bool relationshipInit{ addRelationship(characterId) };

		if (!relationshipInit) {
			return false;
		}

		relationWithCharacter->second[relationId] = 0;
	}
	
	relationWithCharacter->second[relationId] += amountToAdd;
	return true;
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
