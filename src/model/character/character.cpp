

#include "texture.h"
#include "id.h"
#include "character.h"
#include "relation.h"

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

Character::textureId Character::getTextureId(int textureIndex) {
	if (textureIndex < 0 || textureIndex >= m_texturePaths.size()) {
		throw std::out_of_range{ "Texture index out of range" };
	}

	textureId returnId{ 0 };
	std::string& texturePath{ m_texturePaths[textureIndex] };

	runtimeTextureMap::iterator textureEntry{ m_textureLookup.find(texturePath) };

	if (textureEntry != m_textureLookup.end()) {
		returnId = textureEntry->second;
	}
	else {
		textureId id{ TextureManager::registerTexture(texturePath) };

		m_textureLookup[texturePath] = id;
		returnId = id;
	}

	return returnId;
}

std::ostream& operator<<(std::ostream& out, Character& character) {
	out << "Character:" << std::endl;
	std::wcout << "\tname      - " << character.m_name << std::endl;
	out << "\tid        - " << character.m_id << std::endl;

	return out;
}
