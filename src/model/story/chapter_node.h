#ifndef CHAPTER_NODE_H
#define CHAPTER_NODE_H

#include "node.h"

#include "character.h"
#include "state_subject.h"

#include <string>
#include <list>

 //class chaternodestep


class ChapterNode : public Node {
private:
	std::list<std::list<int>> actions{}; // list to contain actions needed to be taked at each step
	std::vector<Character*> m_characters{};
	int m_charactersSpeakerIndex{};
	std::string m_text{};

	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)
public:
	ChapterNode() : Node() {}
	~ChapterNode() { std::cout << "child destructor" << std::endl; };
	ChapterNode(std::string tempData) : Node(tempData) {}

	void addCharacter(Character* character) {
		if (character == nullptr) {
			return;
		}

		m_characters.push_back(character);
	}



	void action(StateSubject* stateSubject) {
		if (stateSubject == nullptr) {
			return;
		}

		stateSubject->updateCurrentText("Test", m_temp);
	}

public:
	// Engine operations
	void addStep() {

	}

public:
	virtual void print(bool printChildren) {
		std::cout << "chapter node" << std::endl;
		std::cout << "id:		 " << m_id << std::endl;
		std::cout << "temp data: " << m_temp << std::endl;
	}
};

#endif // CHAPTER_NODE_H
