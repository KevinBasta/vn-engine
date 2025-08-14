#ifndef VN_CHARACTER_BUILDER_H
#define VN_CHARACTER_BUILDER_H

#include "character.h"

#include <string>

#include <glm/glm.hpp>

class CharacterBuilder {
private:
	Character* m_character{ nullptr };

public:
	CharacterBuilder() : m_character{ new Character{} } {}
	CharacterBuilder(Character* character) : m_character{ character } {}
	~CharacterBuilder() {}

	CharacterBuilder& setName(std::wstring name) {
		if (m_character == nullptr) { std::cout << "CHARACTER NULL" << std::endl; return *this; }

		m_character->m_name = name;

		return *this;
	}

	CharacterBuilder& setTextColor(glm::vec3 color) {
		if (m_character == nullptr) { std::cout << "CHARACTER NULL" << std::endl; return *this; }

		m_character->m_textColor = color;
		
		return *this;
	}

	/*CharacterBuilder& setId(id newid) {
		if (m_character == nullptr) { std::cout << "CHARACTER NULL" << std::endl; return *this; }

		m_character->m_id = newid;

		return *this;
	}*/

	CharacterBuilder& reset() {
		if (m_character == nullptr) { std::cout << "CHARACTER NULL" << std::endl; return *this; }

		m_character->m_name = L"";
		m_character->m_textColor = glm::vec3();
		
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
