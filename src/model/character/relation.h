
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

	// The character who's this relationship object belongs to
	int m_characterId{};
	CharacterToRelationMap m_relationships{};

	bool addRelationship(int characterId);

public: 
	Relations(int characterId) : m_characterId{ characterId }, m_relationships{} { }
	int getCharacterId() { return m_characterId; }

	// characterId: the other character whome this relationship is with
	// relationId: the relation type

	int  getCharacterRelation(int characterId, int relationId);
	bool setCharacterRelation(int characterId, int relationId, int amount);

	bool addCharacterRelation(int characterId, int relationId, int amountToAdd);
	// devide and multiple relation functions?

	friend std::ostream& operator<<(std::ostream& out, Relations& relation);
};

#endif // RELATION_H