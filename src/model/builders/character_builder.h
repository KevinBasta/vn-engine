#ifndef VN_CHARACTER_BUILDER_H
#define VN_CHARACTER_BUILDER_H

#include "character.h"

#include <string>

#include <glm/glm.hpp>

class CharacterBuilder {
private:
	Character* m_character{ nullptr };

public:
	CharacterBuilder() : m_character{ new Character{} } { std::cout << "CHARACTER BUILDRE CONSTRUCTOR" << std::endl; }
	CharacterBuilder(Character* character) : m_character{ character } { std::cout << "CHARACTER BUILDRE CONSTRUCTOR" << std::endl; }
	~CharacterBuilder() { std::cout << "CHARACTER BUILDER DESTRUCTOR" << std::endl; }

	CharacterBuilder& setName(std::wstring name) {
		if (m_character == nullptr) { return *this; }

		m_character->m_name = name;

		return *this;
	}

	CharacterBuilder& setTextColor(glm::vec3 color) {
		if (m_character == nullptr) { return *this; }

		m_character->m_textColor = color;
		
		return *this;
	}

	Character* get() {
		return m_character;
	}

public:
	//
	// Loading and saving node model files
	//
	void save();
	void load();
};

#endif // VN_CHARACTER_BUILDER_H
