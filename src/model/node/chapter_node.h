#ifndef CHAPTER_NODE_H
#define CHAPTER_NODE_H

#include "node.h"

#include "character.h"
#include "model_common.h"

#include <string>
#include <vector>


class StateSubject;

//class chaternodestep
enum class ChapterNodeActionType {
	TYPE_TEXT,
	CHANGE_SPRITE,
	CHANGE_BACKGROUND,
	MOVE_SPRITE,
};

struct ChapterNodeText {
	int characterID{};
	std::string line{};
	
	bool overrideSpeakerName{};
	std::string speakerName{};
};

struct ChapterNodeBackground {
	int backgroundIndex{};
	// replace with index and centralize backgrounds
};

struct ChapterNodeSprite {
	int characterID{};
	int spriteIndex{};
	bool isActive{};
};

class ChapterNode : public Node {
private:
	// temp constructions here, will be done by engine/hooks
	std::vector<std::vector<ChapterNodeActionType>> m_steps{ 
		std::vector<ChapterNodeActionType>{ ChapterNodeActionType::CHANGE_BACKGROUND }, 
		std::vector<ChapterNodeActionType>{ ChapterNodeActionType::TYPE_TEXT, ChapterNodeActionType::CHANGE_SPRITE } 
	};

	std::vector<std::vector<ChapterNodeSprite>> m_spriteSteps{ 
		std::vector<ChapterNodeSprite>{ChapterNodeSprite{0, 1, true}}
	};
	
	std::vector<std::vector<ChapterNodeBackground>> m_backgroundSteps{
		std::vector<ChapterNodeBackground>{ChapterNodeBackground{0}}
	};

	std::vector<std::vector<ChapterNodeText>> m_textSteps{
		std::vector<ChapterNodeText>{ChapterNodeText{0, "hello, this is garu", false, ""}}
	};

	std::string m_text{};

	void doStep(StateSubject* stateSubject, int stepIndex) {
		// execture every action in current iter step
		std::vector<ChapterNodeActionType>& stepActions{ m_steps[stepIndex] };

		std::vector<ChapterNodeActionType>::iterator actionIter;
		for (actionIter = stepActions.begin(); actionIter < stepActions.end(); actionIter++) {
			switch (*actionIter)
			{
			case ChapterNodeActionType::TYPE_TEXT:
				std::vector<ChapterNodeText>::iterator textIterator;

				for (textIterator = m_textSteps[stepIndex].begin(); textIterator < m_textSteps[stepIndex].end(); textIterator++)
				{
					stateSubject->handle(*textIterator);
				}
				break;
			case ChapterNodeActionType::CHANGE_SPRITE:

				break;
			case ChapterNodeActionType::CHANGE_BACKGROUND:

				break;
			default:
				break;
			}
		}

		// call notify on state
	}

	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)
public:
	ChapterNode() : Node() {}
	~ChapterNode() { std::cout << "child destructor" << std::endl; };
	ChapterNode(std::string tempData) : Node(tempData) {}



	NodeState action(StateSubject* stateSubject, int stepIndex);

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
