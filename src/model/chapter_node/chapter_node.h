#ifndef CHAPTER_NODE_H
#define CHAPTER_NODE_H

#include "node.h"

#include "character.h"
#include "chapter_node_types.h"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

class StateSubject;
class ChapterNodeBuilder;
class VnEngineNodeEditor;

class ChapterNode : public Node {
private:
	friend class ChapterNodeBuilder;
	friend class VnEngineNodeEditor;

	// TODO: need to determine what keeps it's state between steps and nodes and what doesn't 
	// (e.g. text is only retained in node, when a new node comes, the text state is reset)

	int m_totalSteps{ 4 };

	// temp data for testing here, will be done by engine/hooks
	std::vector<std::vector<ChapterNodeActionType>> m_steps{ 
		{ ChapterNodeActionType::BACKGROUND, ChapterNodeActionType::SPRITE, ChapterNodeActionType::RELATION },
		{ ChapterNodeActionType::TEXT, ChapterNodeActionType::SPRITE },
		{ ChapterNodeActionType::TEXT, ChapterNodeActionType::SPRITE },
		{ ChapterNodeActionType::CHOICE } // can auto add this for engine
	};

	using StepIndex = int;
	using SubStepIndex = int;
	
	//
	// Background actions
	//

	std::unordered_map<StepIndex, std::vector<ActionBackgroundTexture>> m_backgroundSteps {
		{ 0, { {2, 0} } }
	};


	//
	// Sprite actions
	//


	// TODO: consider a way to make the z index more intuative? maybe the background should be 
	// set at a specific z index (or setable by user) then the rest of the z > than that 
	// is usable for sprites?

	std::unordered_map<StepIndex, std::vector<ActionSpriteProperty>> m_spriteTextureSteps{
		{ 0, std::vector<ActionSpriteProperty>{ {{1, 0}, SpriteProperty::OPACITY, 0.0f}} },
		{ 1, std::vector<ActionSpriteProperty>{ {{1, 0}, SpriteProperty::OPACITY, 1.0f},
												{{1, 0}, SpriteProperty::XPOS, 100.0f},
												{{1, 0}, SpriteProperty::YPOS, -100.0f},
												{{1, 0}, SpriteProperty::ZPOS, -1.0f},
												{{1, 0}, SpriteProperty::SCALE, 1.0f} }
		}
	};

	std::unordered_map<StepIndex, std::vector<ActionSpriteAnimationGeneric>> m_spriteGenericAnimationSteps{
		{ 2, std::vector<ActionSpriteAnimationGeneric>{ {{1, 0}, SpriteProperty::XPOS, {{0.1f, 1000.0f}, {0.2f, 100.0f}} },
														{{1, 0}, SpriteProperty::YPOS, {{0.1f, 200.0f} , {0.2f, -100.0f}} },
														{{1, 0}, SpriteProperty::OPACITY, {{3.0f, 0.3f}} },
														//{{1, 0}, SpriteProperty::SCALE, {{3.1f, 0.5f}} },
													  }
		}
	};


	//
	// Text actions
	//

	std::unordered_map<StepIndex, std::vector<ActionTextRender>> m_textRenderSteps{
		{ 0, {{ false }} },
		{ 1, {{ true }} },
		{ 3, {{ false }} },
	};

	std::unordered_map<StepIndex, std::vector<ActionTextLine>> m_textLineSteps {
		{ 1, {{1, L"hello, this is garu! I came from a far away land to do something important. That is to foil Brazazaza. Why you ask? Well... it's because he... he... well I am actually not very sure myself."}} },
		{ 2, {{1, L"hello, this is NOT garu"}} },
		//{ 3, std::vector<ActionTextLine>{{1, L"hello, this is a potato"}} }
	};	
	
	std::unordered_map<StepIndex, std::vector<ActionTextOverrideSpeaker>> m_textOverrideSpeakerSteps{
		{ 2, {{L"???"}} },
		//{ 3, std::vector<ActionTextOverrideSpeaker>{{L"potato man"}}}
	};

	std::unordered_map<StepIndex, std::vector<ActionTextOverrideColor>> m_textOverrideColorSteps{
		{ 2, {{glm::vec3(0.0f, 1.0f, 0.5f)}} }
	};

	
	//
	// Relation mutation actions
	//

	std::unordered_map<StepIndex, std::vector<ActionRelationModify>> m_relationshipModifySteps{
		{0, {{{1, 2, 1}, RelationModification::ADD, 10}} }
	};

	std::unordered_map <StepIndex, std::vector<ActionRelationSetNextNode>> m_relationshipChooseNode {
		//{0, {1, {}}}
	};

	std::unordered_map<StepIndex, std::vector<ActionRelationSetNextChapter>> m_relationshipChooseChapter{

	};

	//
	// Direct next chapter set
 	//

	std::unordered_map<StepIndex, std::vector<ActionSetNextChapter>> m_setNextChapter{

	};

	//
	// Choice step
	//

	std::unordered_map<StepIndex, std::vector<ActionChoice>> m_choiceTextOptions {
		{3, {{ ChoiceStyle::LIST_TEXT_AREA, {L"second first-level node", L"third first-level node", L"first first-level node"}}} }
	};
	
	std::unordered_map<StepIndex, std::vector<ActionChoiceSetNextNode>> m_choiceSetNextNode {
		{ 3, {{ {{0, 5}, {1, 4}, {2, 2}} }} }
	};


	std::unordered_map<StepIndex, std::vector<ActionChoiceModifyRelation>> m_choiceRelationModifications {
		{3, {{ { {0, { {{1, 2, 1}, RelationModification::ADD, 100} } }}}}}
	};

	std::unordered_map<StepIndex, std::vector<ActionChoiceSetNextChapter>> m_choiceSetNextChapter{

	};



	//
	// Handlers
	//
	bool doStep(StateSubject* stateSubject, int stepIndex);
	
	template <class T>
	bool handle(StateSubject* stateSubject, StepIndex stepIndex, std::unordered_map<StepIndex, std::vector<T>>& stepMap);

	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)
public:
	ChapterNode() : Node() {}
	~ChapterNode() { std::cout << "child destructor" << std::endl; };
	ChapterNode(std::string tempData) : Node(tempData) {}

	NodeState action(StateSubject* stateSubject, int stepIndex);
	index getTotalSteps();

public:
	// Engine operations
	void addStep() {}
	void updateTotalSteps() {}

public:
	virtual void print(bool printChildren) {
		std::cout << "chapter node" << std::endl;
		std::cout << "id:		 " << m_id << std::endl;
		std::cout << "temp data: " << m_temp << std::endl;
	}
};

#endif // CHAPTER_NODE_H
