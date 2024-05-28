
#ifndef CHAPTER_H
#define CHAPTER_H

#include "graph.h"
#include "character.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>

class Chapter {
private:
	std::string m_name{};
	Graph m_graph{};
	int m_id{};

public:
	Chapter() {};

	// Setters
	//void addCharacter(Character& character) { m_characterSceneData[character] = character; };

	// Getters
	Graph& getGraph() { return m_graph; }


};

#endif // CHAPTER_H
