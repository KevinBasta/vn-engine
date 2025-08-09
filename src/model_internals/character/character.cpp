

#include "texture.h"
#include "id.h"
#include "character.h"

#include <iostream>
#include <string>
#include <string_view>
#include <memory>

Character::Character() :
	m_id{ IdGenerator<Character>::getId() } 
{
	std::wcout << "constructing character" << std::endl;
}

Character::Character(id userSetId) :
	m_id{ userSetId }
{
	IdGenerator<Character>::setIdToAfter(userSetId);
}

Character::Character(const Character& character) :
	m_id{ IdGenerator<Character>::getId() }
{
	m_name = character.m_name;
	m_textColor = character.m_textColor;
}

Character::~Character() {
	std::wcout << "deleting character " << m_name << std::endl;
}

std::ostream& operator<<(std::ostream& out, Character& character) {
	out << "Character:" << std::endl;
	std::wcout << "\tname      - " << character.m_name << std::endl;
	out << "\tid        - " << character.m_id << std::endl;

	return out;
}
