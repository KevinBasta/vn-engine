

#include "texture.h"
#include "id.h"
#include "character.h"
#include "relation.h"

#include <iostream>
#include <string>
#include <string_view>
#include <memory>

Character::Character(std::wstring name) :
	m_name{ name },
	m_id{ IdGenerator<Character>::getId() } 
{
	std::wcout << "constructing character " << m_name << std::endl;
}

Texture2D* Character::getTexture(int index) {
	// TODO: handle negative index?
	return m_textures[index].get();
}

void Character::addTexture(const char* texturePath) {
	m_textures.push_back(std::make_unique<Texture2D>(texturePath));
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
