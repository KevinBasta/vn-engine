
#ifndef CHARACTER_H
#define CHARACTER_H

#include "id.h"
#include "texture_store.h"

#include <memory>

#include <vector>
#include <unordered_map>

#include <string>
#include <string_view>
#include <iostream>

#include <glm/glm.hpp>

class CharacterBuilder;

class Character {
private:
	friend class CharacterBuilder;

	const id m_id{};
	std::wstring m_name{};
	glm::vec3 m_textColor{0.4f, 0.3f, 0.9f};
	
public:
	Character();
	Character(id userSetId);
	Character(const Character& character);
	~Character();

	id getId() const { return m_id; }
	std::wstring getName() { return m_name; }
	glm::vec3 getTextColor() const { return m_textColor; }

	friend std::ostream& operator<<(std::ostream& out, Character& character);
};

#endif // CHARACTER_H