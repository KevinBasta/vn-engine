
#ifndef CHARACTER_H
#define CHARACTER_H

#include "texture.h"
#include "relation.h"
#include "id.h"

#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <memory>

class Character {
private:
	const int	m_id;

	std::wstring m_name{};
	glm::vec3 m_textColor{0.4f, 0.3f, 0.9f};
	std::vector<std::unique_ptr<Texture2D>> m_textures{};

public:
	Character(std::wstring name);
	Character(std::wstring name, Relations relationships);

	void addTexture(const char* texturePath);
	
	Texture2D* getTexture(int index);
	int getId() { return m_id; }
	std::wstring_view getName() { return m_name; }
	glm::vec3 getTextColor() { return m_textColor; }

	~Character();

	friend std::ostream& operator<<(std::ostream& out, Character& character);

	// vector  of affection int (generalized to emotion object to hold differnet emotions) to a reference of a player and their affection number?	
};

#endif // CHARACTER_H