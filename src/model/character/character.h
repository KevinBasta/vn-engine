
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
	std::string m_name{};
	const int	m_id;
	std::vector<std::unique_ptr<Texture2D>> m_textures{};

public:
	Character(std::string_view name);
	Character(std::string_view name, Relations relationships);

	void addTexture(const char* texturePath);
	Texture2D* getTexture(int index);
	int getId() { return m_id; }

	~Character();

	friend std::ostream& operator<<(std::ostream& out, Character& character);

	// vector  of affection int (generalized to emotion object to hold differnet emotions) to a reference of a player and their affection number?	
};

#endif // CHARACTER_H