#ifndef CHAPTER_NODE_H
#define CHAPTER_NODE_H

#include "id.h"
#include "index.h"

#include "node.h"

#include "character.h"
#include "chapter_node_types.h"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

class StateSubject;
class ChapterNodeBuilder;
class VnEngineSceneEditor;

class ChapterNode : public Node {
private:
	friend class ChapterNodeBuilder;

	template <class T>
	using ActionStepMap = std::unordered_map<index, std::vector<T>>;

	// TODO: need to determine what keeps it's state between steps and nodes and what doesn't 
	// (e.g. text is only retained in node, when a new node comes, the text state is reset)

	int m_totalSteps{ 4 };

	// temp data for testing here, will be done by engine/hooks
	// can auto add this for engine
	/*std::vector<std::vector<ChapterNodeActionType>> m_steps{ 
		{ ChapterNodeActionType::BACKGROUND, ChapterNodeActionType::SPRITE, ChapterNodeActionType::RELATION },
		{ ChapterNodeActionType::TEXT, ChapterNodeActionType::SPRITE },
		{ ChapterNodeActionType::TEXT, ChapterNodeActionType::SPRITE },
		{ ChapterNodeActionType::CHOICE } 
	};*/

	
	//
	// Background actions
	//

	ActionStepMap<ActionBackgroundTexture> m_backgroundSteps {
		{ 0, { {2, 0} } }
	};


	//
	// Sprite actions
	//


	// TODO: consider a way to make the z index more intuative? maybe the background should be 
	// set at a specific z index (or setable by user) then the rest of the z > than that 
	// is usable for sprites?
	ActionStepMap<ActionSpriteAllProperties> m_spritePropertySteps{
		{ 0, { {{1, 0}, false, 0, false, 0, false, 0, false, 0, false, 0, true, 0.0f}} },
		{ 1, { {{1, 0}, true, 100.0f, true, -100.0f, true, -1.0f, true, 1.0f, false, 0, true, 1.0f }} },
	};

	ActionStepMap<ActionSpriteAnimation> m_spriteAnimationSteps{
		{ 2, { 
				{
					{1, 0}, 
					true, {{0.1f, 1000.0f}, {0.2f, 100.0f}},
					true, {{0.1f, 200.0f} , {0.2f, -100.0f}},
					false, {}, false, {}, false, {},
					true, {{3.0f, 0.3f}}
				}
			}
		}
	};

	//
	// Text actions
	//
	ActionStepMap<ActionTextLine> m_textLineSteps {
		{ 0, {{false, 1, L"\tありがとうaaa"}} },
		{ 1, {{false, 1, L"hello, this is garu! I came from a far away land to do something important. That is to foil Brazazaza. Why you ask? Well... it's because he... he... well I am actually not very sure myself."}} },
		{ 2, {{false, 1, L"hello, this is NOT garu"}} },
		//{ 3, std::vector<ActionTextLine>{{1, L"hello, this is a potato"}} }
	};	
	
	ActionStepMap<ActionTextOverrides> m_textOverrideSteps{ 
		{0, {{true, true, true, L"???", true, glm::vec3(1.0f, 0.0f, 0.0f)}}},
		{1, {{true, true}}},
		{2, {{false, true, true, L"???", true, glm::vec3(0.0f, 1.0f, 0.5f)}}},
		{3, {{true, false}}}
	};
	
	//
	// Relation mutation actions
	//

	ActionStepMap<ActionRelationModify> m_relationshipModifySteps{
		{0, {{{1, 2, 1}, RelationModification::ADD, 10}} }
	};

	ActionStepMap<ActionRelationSetNextNode> m_relationshipChooseNode { };

	ActionStepMap<ActionRelationSetNextChapter> m_relationshipChooseChapter{ };

	//
	// Direct next chapter set
 	//

	ActionStepMap<ActionSetNextChapter> m_setNextChapter{ };

	//
	// Choice step
	//

	ActionStepMap<ActionChoice> m_choiceTextOptions {
		{3, {{ ChoiceStyle::LIST_TEXT_AREA, {L"second first-level node", L"third first-level node", L"first first-level node"}}} }
	};
	
	ActionStepMap<ActionChoiceSetNextNode> m_choiceSetNextNode {
		{ 3, {{ {{0, 5}, {1, 4}, {2, 2}} }} }
	};


	ActionStepMap<ActionChoiceModifyRelation> m_choiceRelationModifications {
		{3, {{ { {0, { {{1, 2, 1}, RelationModification::ADD, 100} } }}}}}
	};

	ActionStepMap<ActionChoiceSetNextChapter> m_choiceSetNextChapter{ };



	//
	// Handlers
	//
	bool doStep(StateSubject* stateSubject, int stepIndex);
	
	template <class T>
	bool handle(StateSubject* stateSubject, index stepIndex, ActionStepMap<T>& stepMap);

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
	void addStep() {}
	void updateTotalSteps() {}
	int getTotalSteps() { return m_totalSteps; }
	int getLastStepIndex() { return m_totalSteps - 1; }

public:
	virtual void print(bool printChildren) {
		std::cout << "chapter node" << std::endl;
		std::cout << "id:		 " << m_id << std::endl;
		std::cout << "temp data: " << m_name << std::endl;
	}
};

#endif // CHAPTER_NODE_H
