

#include "id.h"
#include "character.h"
#include "relation.h"

#include <iostream>
#include <string>
#include <string_view>

Character::Character(std::string_view name) :
	m_name{ name },
	m_id{ IdGenerator::getId() } 
{

}

Character::Character(std::string_view name, Relation relationships) :
	m_name{ name },
	m_id{ IdGenerator::getId() },
	m_relationships{ relationships }
{

}


std::ostream& operator<<(std::ostream& out, Character character) {
	std::cout << "Character:" << std::endl;
	std::cout << "\t name      - " << character.m_name << std::endl;
	std::cout << "\t id        - " << character.m_id << std::endl;
	std::cout << "\t relations - " << character.m_relationships << std::endl;

	return out;
}
