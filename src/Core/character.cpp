

#include "texture.h"
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

void Character::addTexture(const char* texturePath) {
	OpenGLTexture newTexture{ texturePath };
	m_textures.push_back(newTexture);
}

Character::~Character() {
	std::cout << "deleting character " << m_name << std::endl;
}


std::ostream& operator<<(std::ostream& out, Character& character) {
	out << "Character:" << std::endl;
	out << "\tname      - " << character.m_name << std::endl;
	out << "\tid        - " << character.m_id << std::endl;
	out << "\t"<< character.m_relationships << std::endl;

	return out;
}
