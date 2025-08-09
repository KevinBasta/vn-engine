

#include "index.h"

#include "node.h"
#include "node_runner.h"
#include "state_subject.h"
#include "node_types.h"

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <functional>

template<typename> struct stateHelper;

template<> struct stateHelper<ActionBackgroundTexture> { static constexpr auto handler = &StateSubject::m_background; };

template<> struct stateHelper<ActionSpriteAllProperties> { static constexpr auto handler = &StateSubject::m_sprites; };
template<> struct stateHelper<ActionSpriteAnimation> { static constexpr auto handler = &StateSubject::m_sprites; };

template<> struct stateHelper<ActionTextLine> { static constexpr auto handler = &StateSubject::m_dialogue; };
template<> struct stateHelper<ActionTextOverrides> { static constexpr auto handler = &StateSubject::m_dialogue; };

template<> struct stateHelper<ActionRelationModify> { static constexpr auto handler = &StateSubject::m_relations; };
template<> struct stateHelper<ActionRelationSetNextNode> { static constexpr auto handler = &StateSubject::m_relations; };
template<> struct stateHelper<ActionRelationSetNextChapter> { static constexpr auto handler = &StateSubject::m_relations; };

template<> struct stateHelper<ActionSetNextChapter> { static constexpr auto handler = &StateSubject::m_nextChapter; };

template<> struct stateHelper<ActionChoice> { static constexpr auto handler = &StateSubject::m_choices; };
template<> struct stateHelper<ActionChoiceSetNextNode> { static constexpr auto handler = &StateSubject::m_choices; };
template<> struct stateHelper<ActionChoiceModifyRelation> { static constexpr auto handler = &StateSubject::m_choices; };
template<> struct stateHelper<ActionChoiceSetNextChapter> { static constexpr auto handler = &StateSubject::m_choices; };


NodeRunner Node::iter() { return NodeRunner(this); }


std::ostream& operator<<(std::ostream& out, Node& node) {
	std::cout << "a node" << std::endl;

	return out;
}

//
// Step Handling
// Sends data to the subject
// Bool returns indicate if any step occured
//
template <class T>
bool Node::handle(
	StateSubject* stateSubject,
	index stepIndex,
	ActionStepMap<T>& stepMap)
{
	bool hasStep{ false };

	if (stateSubject == nullptr) {
		return hasStep;
	}

	class ActionStepMap<T>::const_iterator stepLocation = stepMap.find(stepIndex);

	// Check if the step exsists for this type of action
	if (stepLocation != stepMap.end()) {
		hasStep = true;
		class std::vector<T>::const_iterator action;

		// Execute all the actions within the step
		for (action = stepLocation->second.begin(); action < stepLocation->second.end(); action++)
		{
			(stateSubject->*(stateHelper<T>::handler)).handle(*action);
		}
	}

	return hasStep;
}

bool Node::doStep(StateSubject* stateSubject, int stepIndex) {
	// Exectute every action in current iter step, order matters
	bool stepExists{ false };

	if (stateSubject == nullptr) {
		return false;
	}

	stepExists |= handle(stateSubject, stepIndex, m_backgroundSteps);

	stepExists |= handle(stateSubject, stepIndex, m_spritePropertySteps);
	stepExists |= handle(stateSubject, stepIndex, m_spriteAnimationSteps);

	stepExists |= handle(stateSubject, stepIndex, m_textLineSteps);
	stepExists |= handle(stateSubject, stepIndex, m_textOverrideSteps);

	stepExists |= handle(stateSubject, stepIndex, m_relationshipModifySteps);
	stepExists |= handle(stateSubject, stepIndex, m_relationshipChooseNode);
	//stepExists |= handle(stateSubject, stepIndex, m_relationshipChooseChapter);

	stepExists |= handle(stateSubject, stepIndex, m_setNextChapter);
	stepExists |= handle(stateSubject, stepIndex, m_choiceTextOptions);
	stepExists |= handle(stateSubject, stepIndex, m_choiceSetNextNode);
	stepExists |= handle(stateSubject, stepIndex, m_choiceRelationModifications);
	stepExists |= handle(stateSubject, stepIndex, m_choiceSetNextChapter);



	//if (stepIndex >= m_steps.size()) {
	//	return false;
	//}
	//
	//std::vector<NodeActionType>& steps{ m_steps[stepIndex] };
	//std::vector<NodeActionType>::iterator step { steps.begin() };

	//for (; step != steps.end(); step++) {
	//	switch (*step) {
	//	case (NodeActionType::BACKGROUND):
	//		stepExists |= handle(stateSubject, stepIndex, m_backgroundSteps);

	//		break;
	//	case (NodeActionType::SPRITE):
	//		stepExists |= handle(stateSubject, stepIndex, m_spriteTextureSteps);
	//		stepExists |= handle(stateSubject, stepIndex, m_spriteGenericAnimationSteps);
	//		
	//		break;
	//	case (NodeActionType::TEXT):
	//		stepExists |= handle(stateSubject, stepIndex, m_textRenderSteps);
	//		stepExists |= handle(stateSubject, stepIndex, m_textLineSteps);
	//		stepExists |= handle(stateSubject, stepIndex, m_textOverrideSpeakerSteps);
	//		stepExists |= handle(stateSubject, stepIndex, m_textOverrideColorSteps);

	//		break;
	//	case (NodeActionType::RELATION):
	//		stepExists |= handle(stateSubject, stepIndex, m_relationshipModifySteps);
	//		stepExists |= handle(stateSubject, stepIndex, m_relationshipChooseNode);
	//		//stepExists |= handle(stateSubject, stepIndex, m_relationshipChooseChapter);

	//		break;
	//	case (NodeActionType::CHOICE):
	//		stepExists |= handle(stateSubject, stepIndex, m_setNextChapter);
	//		stepExists |= handle(stateSubject, stepIndex, m_choiceTextOptions);
	//		stepExists |= handle(stateSubject, stepIndex, m_choiceSetNextNode);
	//		stepExists |= handle(stateSubject, stepIndex, m_choiceRelationModifications);
	//		stepExists |= handle(stateSubject, stepIndex, m_choiceSetNextChapter);

	//		break;
	//	default:
	//		break;
	//	}
	//}

	return stepExists;
	// Notify called in state after iter step is called
}


//TODO replace stepIndex type with index type?
NodeState Node::action(StateSubject* stateSubject, int stepIndex)
{
	// TODO: Remove m_temp
	std::cout << "test speaker" << " said: " << m_name << std::endl;
	std::cout << "step #" << stepIndex << std::endl;

	//if (stepIndex < m_steps.size()) {
	if (stepIndex < m_totalSteps) {
		bool stepDone = doStep(stateSubject, stepIndex);

		if (stepIndex + 1 < m_totalSteps) {
			return NodeState::NODE_STEP;
		}
		else {
			return NodeState::NODE_END;
		}
	}

	//stateSubject->nodeEndActions();

	//std::cout << "step #" << stepIndex << " " << (stepDone ? "completed" : "not completed") << std::endl;

	return NodeState::NODE_END;
}
