
#ifndef CHARACTER_H
#define CHARACTER_H

#include "id.h"
#include "texture_manager.h"

#include <memory>

#include <vector>
#include <unordered_map>

#include <string>
#include <string_view>
#include <iostream>

#include <stdexcept>

#include <glm/glm.hpp>

class CharacterBuilder;

class Character {
private:
	friend class CharacterBuilder;

	using textureId	   = int;
	using runtimeTextureMap = std::unordered_map<std::string_view, textureId>;

	const int	m_id{};

	std::wstring m_name{};
	glm::vec3 m_textColor{0.4f, 0.3f, 0.9f};
	std::vector<std::string> m_texturePaths{};
	runtimeTextureMap m_textureLookup{};

public:
	Character();
	~Character();

	int getId() const { return m_id; }
	std::wstring_view getName() { return m_name; }
	glm::vec3 getTextColor() const { return m_textColor; }
	textureId getTextureId(int textureIndex);

	friend std::ostream& operator<<(std::ostream& out, Character& character);
};

#endif // CHARACTER_H