
#ifndef CHARACTER_H
#define CHARACTER_H

#include "relation.h"
#include "id.h"

#include <string>
#include <string_view>
#include <iostream>

class Character {
private:
	std::string m_name{};
	const int	m_id;
	Relation	m_relationships{};


public:
	Character(std::string_view name);
	Character(std::string_view name, Relation relationships);

	friend std::ostream& operator<<(std::ostream& out, Character character);

	// vector  of affection int (generalized to emotion object to hold differnet emotions) to a reference of a player and their affection number?	
};

#endif // CHARACTER_H