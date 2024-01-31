
#ifndef RELATION_H
#define RELATION_H

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
	using CharacterId	= int;
	using RelationId	= int;
	using RelationValue = int;
	using RelationToValueMap = std::unordered_map<RelationId, RelationValue>;
	using CharacterToRelationMap = std::unordered_map<CharacterId, RelationToValueMap>;

	// maybe a weak pointer to a shared pointer that is the character?
	CharacterToRelationMap m_relationships{};

public: 
	Relations();

	void addCharacterRelation(int characterId, int relationId, int amountToAdd);

	friend std::ostream& operator<<(std::ostream& out, Relations relation);
};

#endif // RELATION_H