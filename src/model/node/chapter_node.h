#ifndef CHAPTER_NODE_H
#define CHAPTER_NODE_H

#include "node.h"

#include "character.h"
#include "model_common.h"
#include "chapter_node_types.h"

#include <string>
#include <vector>
#include <unordered_map>

class StateSubject;

//class chaternodestep
enum class ChapterNodeActionType {
	TYPE_TEXT,
	CHANGE_SPRITE,
	CHANGE_BACKGROUND,
	MOVE_SPRITE,
};

class ChapterNode : public Node {
private:
	// temp constructions here, will be done by engine/hooks
	/*std::vector<std::vector<ChapterNodeActionType>> m_steps{ 
		std::vector<ChapterNodeActionType>{ ChapterNodeActionType::CHANGE_BACKGROUND, ChapterNodeActionType::CHANGE_SPRITE },
		std::vector<ChapterNodeActionType>{ ChapterNodeActionType::TYPE_TEXT, ChapterNodeActionType::CHANGE_SPRITE },
		std::vector<ChapterNodeActionType>{ ChapterNodeActionType::TYPE_TEXT },
		std::vector<ChapterNodeActionType>{ ChapterNodeActionType::CHANGE_SPRITE }
	};*/

	typedef int StepIndex;
	typedef int SubStepIndex;
	
	std::unordered_map<StepIndex, std::vector<ActionTextLine>> m_textLineSteps{
		{ 1, std::vector<ActionTextLine>{{1, L"hello, this is garu"}} },
		{ 2, std::vector<ActionTextLine>{{1, L"hello, this is NOT garu"}} }
	};	
	
	std::unordered_map<StepIndex, std::vector<ActionTextOverrideSpeaker>> m_textOverrideSpeakerSteps{
		{ 2, std::vector<ActionTextOverrideSpeaker>{{L"???"}}}
	};

	std::unordered_map<StepIndex, std::vector<ActionTextOverrideColor>> m_textOverrideColorSteps{
		{ 2, std::vector<ActionTextOverrideColor>{{glm::vec3(0.0f, 1.0f, 0.5f)}}}
	};





	std::unordered_map<StepIndex, std::vector<ActionSpriteTexture>> m_spriteTextureSteps{
		{ 0, std::vector<ActionSpriteTexture>{{1, 0}} }
	};	
	
	std::unordered_map<StepIndex, std::vector<ActionSpriteOpacity>> m_spriteOpacitySteps{
		{ 0, std::vector<ActionSpriteOpacity>{{1, 0.0f}} },
		{ 1, std::vector<ActionSpriteOpacity>{{1, 1.0f}} }
	};

	// TODO: consider a way to make the z index more intuative? maybe the background should be 
	// set at a specific z index (or setable by user) then the rest of the z > than that 
	// is usable for sprites?
	std::unordered_map<StepIndex, std::vector<ActionSpritePosition>> m_spritePositionSteps{
		{ 1, std::vector<ActionSpritePosition>{{1, 100.0f, -100.0f, -1.0f, 1.0f}} }
	};
	
	std::unordered_map<StepIndex, std::vector<ActionSpriteAnimation>> m_spriteAnimationSteps{
		{ 2, std::vector<ActionSpriteAnimation>{ {1, {{0.5f, 1000.0f, 100.0f, -1.0f, 1.0f}}} } }
	};




	std::unordered_map<StepIndex, std::vector<ActionBackgroundTexture>> m_backgroundSteps{
		{ 0, std::vector<ActionBackgroundTexture>{{0}} }
	};
	
	std::string m_text{};

	template <class T>
	bool handleStep(StateSubject* stateSubject, StepIndex stepIndex, std::unordered_map<StepIndex, std::vector<T>>& stepMap);
	bool doStep(StateSubject* stateSubject, int stepIndex);
	

	template <class T>
	bool handleSubStep(StateSubject* stateSubject, StepIndex stepIndex, std::unordered_map<StepIndex, std::vector<T>>& stepMap);
	bool handeSubStepSpecialized(StateSubject* stateSubject, ActionSpriteAnimation& action);
	

	//template <class T>
	//bool hasStep(StepIndex stepIndex, std::unordered_map<StepIndex, std::vector<T>>& stepMap);
	//int countSteps();
	//int countSubsteps(int stepIndex);

	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)
public:
	ChapterNode() : Node() {}
	~ChapterNode() { std::cout << "child destructor" << std::endl; };
	ChapterNode(std::string tempData) : Node(tempData) {}

	NodeState action(StateSubject* stateSubject, int stepIndex);
	NodeState subAction(StateSubject* stateSubject, int stepIndex, int subStepIndex);

public:
	// Engine operations
	void addStep() {}

public:
	virtual void print(bool printChildren) {
		std::cout << "chapter node" << std::endl;
		std::cout << "id:		 " << m_id << std::endl;
		std::cout << "temp data: " << m_temp << std::endl;
	}
};

#endif // CHAPTER_NODE_H
