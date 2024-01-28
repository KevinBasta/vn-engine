
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

class RelationTypes {
private:
	using RelationName = std::string;
	using RelationId = int;

	static std::unordered_map<RelationName, RelationId> relationTypes;

public:
	static void addRelationType(std::string relationName);
	static void print();
};


class Relations {
private:
	using CharacterId	= int;
	using RelationId	= int;
	using RelationValue = int;
	using Relationship	= std::unordered_map<RelationId, RelationValue>;

	// maybe a weak pointer to a shared pointer that is the character?
	std::unordered_map<CharacterId, Relationship> m_relationships{};

public: 
	Relations();

	friend std::ostream& operator<<(std::ostream& out, Relations relation);
};

#endif // RELATION_H