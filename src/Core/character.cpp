

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
	std::cout << "constructing character " << m_name << std::endl;
}

Character::Character(std::string_view name, Relations relationships) :
	m_name{ name },
	m_id{ IdGenerator::getId() },
	m_relationships{ relationships }
{

}

Character::~Character() {
	std::cout << "deleting character " << m_name << std::endl;
}


std::ostream& operator<<(std::ostream& out, Character& character) {
	out << "Character:" << std::endl;
	out << "\t name      - " << character.m_name << std::endl;
	out << "\t id        - " << character.m_id << std::endl;
	out << "\t "<< character.m_relationships << std::endl;

	return out;
}
