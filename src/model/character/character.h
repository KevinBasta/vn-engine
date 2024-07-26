
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
	TextureStore m_textrues{};

public:
	Character();
	~Character();

	id getId() const { return m_id; }
	std::wstring_view getName() { return m_name; }
	glm::vec3 getTextColor() const { return m_textColor; }
	TextureStore::textureId getTextureIdByIndex(int textureIndex);

	friend std::ostream& operator<<(std::ostream& out, Character& character);
};

#endif // CHARACTER_H