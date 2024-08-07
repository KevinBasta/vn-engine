
#ifndef VN_RELATIONS_H
#define VN_RELATIONS_H

#include "id.h"
#include "chapter_node_types.h"

#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

/*
 * A generalized data class that contians some statuses (e.g. affection)
 * towards the main character. Can be generalized to include for all other characters?
 */

class Relations {
private:
	using CharacterId	= id;
	using RelationId	= id;
	using RelationValue = int;
	using RelationToValueMap = std::unordered_map<RelationId, RelationValue>;
	using CharacterToRelationMap = std::unordered_map<CharacterId, RelationToValueMap>;

	CharacterToRelationMap m_relationships{};

public: 
	Relations() : m_relationships{} { }

	int  getCharacterRelation(int characterId, int relationId);
	bool modifyCharacterRelation(int characterId, int relationId, RelationModification modification, int amount);

	friend std::ostream& operator<<(std::ostream& out, Relations& relation);
};

#endif // VN_RELATIONS_H