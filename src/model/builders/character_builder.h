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

	void setName(std::wstring name) {
		if (m_character == nullptr) { return; }

		m_character->m_name = name;
	}

	void setTextColor(glm::vec3 color) {
		if (m_character == nullptr) { return; }

		m_character->m_textColor = color;
	}

	void addTexture(std::string path) {
		if (m_character == nullptr) { return; }

		m_character->m_textrues.addTexture(path);
	}

	Character* get() {
		return m_character;
	}
};

#endif // VN_CHARACTER_BUILDER_H
