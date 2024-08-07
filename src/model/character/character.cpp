

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

Character::~Character() {
	std::wcout << "deleting character " << m_name << std::endl;
}

TextureStore::textureId Character::getTextureIdByIndex(int textureIndex) {
	return m_textrues.getTextureIdByIndex(textureIndex);
}

std::ostream& operator<<(std::ostream& out, Character& character) {
	out << "Character:" << std::endl;
	std::wcout << "\tname      - " << character.m_name << std::endl;
	out << "\tid        - " << character.m_id << std::endl;

	return out;
}
