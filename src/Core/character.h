
#ifndef CHARACTER_H
#define CHARACTER_H


#include "relation.h"
#include "id.h"


#include <string>

class Character {
public:
	std::string m_name{};
	const Id	m_id;
	Relation	m_relationships{};


private:
	Character() : m_id{ IdGenerator::getId() } {

	}

	// vector  of affection int (generalized to emotion object to hold differnet emotions) to a reference of a player and their affection number?	
};

#endif // CHARACTER_H