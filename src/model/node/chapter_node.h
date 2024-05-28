#ifndef CHAPTER_NODE_H
#define CHAPTER_NODE_H

#include "node.h"

#include "character.h"
#include "state_subject.h"
#include "model_common.h"

#include <string>
#include <list>

 //class chaternodestep
enum class ChapterNodeActionType {
	TYPE_TEXT,
	CHANGE_SPRITE,
	MOVE_SPRITE,
};

class ChapterNode : public Node {
private:
	std::list<std::list<ChapterNodeActionType>> m_steps{};

	std::list<std::pair<int, std::string>> m_typeText{};

	std::vector<Character*> m_characters{};
	int m_charactersSpeakerIndex{};
	std::string m_text{};




	void doStep(StateSubject* stateSubject, int stepIndex) {
		// execture every action in current iter step
		// for (int )



		// call notify on state
	}

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



	NodeState action(StateSubject* stateSubject, int stepIndex) {
		doStep(stateSubject, stepIndex);

		stateSubject->updateCurrentText("Test", m_temp);
		
		if (stepIndex == (m_steps.size() - 1)) {
			if (m_children.size() > 1) {
				return NodeState::NODE_CHOICE;
			}
			else {
				return NodeState::NODE_END;
			}
		}
		else {
			return NodeState::NODE_STEP;
		}
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