
#include "id.h"
#include "relation.h"
#include "relation_types.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

Relations::Relations() : m_relationships{} {

};

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

void Relations::addCharacterRelation(int characterId, int relationId, int amountToAdd) {
	CharacterToRelationMap::iterator relationWithCharacter = m_relationships.find(characterId);
	
	if (relationWithCharacter != m_relationships.end()) {
		// If the character is already in the relation map

		RelationToValueMap::iterator relationshipEntry = relationWithCharacter->second.find(relationId);
			
		if (relationshipEntry != relationWithCharacter->second.end()) {
			// If relation type entry exists, add onto it
			relationshipEntry->second += amountToAdd;
		}
		else {
			// If relation type entry does not exist, create it
			relationWithCharacter->second.insert(std::make_pair(relationId, amountToAdd));
		}
	}
	else {
		// If the character is not in the relation map

		// Initilize the relationship with the character
		auto [relationshipEntry, result] = m_relationships.insert(std::make_pair(characterId, RelationToValueMap{}));
		
		if (result) {
			// Add the relation id and value
			relationshipEntry->second.insert(std::make_pair(relationId, amountToAdd));
		}
		else {
			std::cout << "error creating character relation entry" << std::endl;
		}
	}
}
