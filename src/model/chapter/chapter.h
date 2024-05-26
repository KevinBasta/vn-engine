
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
	StateSubject* m_stateSubject{ nullptr };

	std::string m_name{};
	Graph m_graph{};
	int m_id{};

public:
	Chapter() {};

	void attatchControllingObject(StateSubject* stateSubject) {
		m_stateSubject = stateSubject;
	}

	void forwardProgress() {
		m_graph.forwardProgress(m_stateSubject);
	}


	// Setters
	//void addCharacter(Character& character) { m_characterSceneData[character] = character; };

	// Getters
	Graph& getGraph() { return m_graph; }


};

#endif // CHAPTER_H
