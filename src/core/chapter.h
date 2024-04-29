
#ifndef CHAPTER_H
#define CHAPTER_H

#include "graph.h"
#include "character.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>

struct CharacterSceneData {
	Texture2D* currentTexture;
	float scaleValue{ 1.0f };
	glm::mat4 model{ glm::mat4(1.0f) };
	bool onScreen{ false };

	CharacterSceneData(Character& character) : 
		currentTexture{ &(character.getTextures()[0]) }
	{
	
	};
};

// Scene camera/projection data
/*
struct SceneCameraData {

};
*/

class Chapter {
private:
	std::string m_name{};
	int m_id{};
	Graph m_graph;
	//std::unordered_map<Character&, CharacterSceneData> m_characterSceneData{};
	// background variable can be defined here too

public:
	Chapter();

	// Setters
	//void addCharacter(Character& character) { m_characterSceneData[character] = character; };

	// Getters
	Graph& getGraph() { return m_graph; }


};

#endif // CHAPTER_H
