

#include "chapter_node.h"
#include "state_subject.h"
#include "chapter_node_types.h"

#include <list>
#include <vector>
#include <unordered_map>
#include <iterator>

#include <functional>


template<typename> struct stateHelper;

template<> struct stateHelper<ActionBackgroundTexture>		{ static constexpr auto handler = &StateSubject::m_background; };

template<> struct stateHelper<ActionSpriteTexture>			{ static constexpr auto handler = &StateSubject::m_sprites; };
template<> struct stateHelper<ActionSpriteOpacity>			{ static constexpr auto handler = &StateSubject::m_sprites; };
template<> struct stateHelper<ActionSpritePosition>			{ static constexpr auto handler = &StateSubject::m_sprites; };
template<> struct stateHelper<ActionSpriteAnimationGeneric>	{ static constexpr auto handler = &StateSubject::m_sprites; };

template<> struct stateHelper<ActionTextRender>				{ static constexpr auto handler = &StateSubject::m_dialogue; };
template<> struct stateHelper<ActionTextLine>				{ static constexpr auto handler = &StateSubject::m_dialogue; };
template<> struct stateHelper<ActionTextOverrideSpeaker>	{ static constexpr auto handler = &StateSubject::m_dialogue; };
template<> struct stateHelper<ActionTextOverrideColor>		{ static constexpr auto handler = &StateSubject::m_dialogue; };

template<> struct stateHelper<ActionRelationModify>			{ static constexpr auto handler = &StateSubject::m_relations; };
template<> struct stateHelper<ActionRelationSetNextNode>	{ static constexpr auto handler = &StateSubject::m_relations; };

template<> struct stateHelper<ActionChoice>					{ static constexpr auto handler = &StateSubject::m_choices; };
template<> struct stateHelper<ActionChoiceSetNextNode>		{ static constexpr auto handler = &StateSubject::m_choices; };
template<> struct stateHelper<ActionChoiceModifyRelation>	{ static constexpr auto handler = &StateSubject::m_choices; };

//
// Step Handling
// Sends data to the subject
// Bool returns indicate if any step occured
//
template <class T>
bool ChapterNode::handle(
	StateSubject* stateSubject,
	StepIndex stepIndex,
	std::unordered_map<StepIndex, T>& stepMap)
{
	bool hasStep{ false };

	if (stateSubject == nullptr) {
		return hasStep;
	}

	class std::unordered_map<StepIndex, T>::iterator stepLocation = stepMap.find(stepIndex);

	// Check if the step exsists for this type of action
	if (stepLocation != stepMap.end()) {
		hasStep = true;

		(stateSubject->*(stateHelper<T>::handler)).handle(stepLocation->second);
	}

	return hasStep;
}

template <class T>
bool ChapterNode::handle(
	StateSubject *stateSubject,
	StepIndex stepIndex, 
	std::unordered_map<StepIndex, std::vector<T>>& stepMap)
{
	bool hasStep{ false };

	if (stateSubject == nullptr) {
		return hasStep;
	}

	class std::unordered_map<StepIndex, std::vector<T>>::iterator stepLocation = stepMap.find(stepIndex);

	// Check if the step exsists for this type of action
	if (stepLocation != stepMap.end()) {
		hasStep = true;
		class std::vector<T>::iterator action;

		// Execute all the actions within the step
		for (action = stepLocation->second.begin(); action < stepLocation->second.end(); action++)
		{
			(stateSubject->*(stateHelper<T>::handler)).handle(*action);
		}
	}

	return hasStep;
}

bool ChapterNode::doStep(StateSubject* stateSubject, int stepIndex) {
	// Exectute every action in current iter step, order matters
	bool stepExists{ false };

	if (stateSubject == nullptr) {
		return false;
	}

	if (stepIndex >= m_steps.size()) {
		return false;
	}
	
	std::vector<ChapterNodeActionType>& steps{ m_steps[stepIndex] };
	std::vector<ChapterNodeActionType>::iterator step { steps.begin() };

	for (; step != steps.end(); step++) {
		switch (*step) {
		case (ChapterNodeActionType::BACKGROUND):
			stepExists |= handle(stateSubject, stepIndex, m_backgroundSteps);

			break;
		case (ChapterNodeActionType::SPRITE):
			stepExists |= handle(stateSubject, stepIndex, m_spriteTextureSteps);
			stepExists |= handle(stateSubject, stepIndex, m_spriteOpacitySteps);
			stepExists |= handle(stateSubject, stepIndex, m_spritePositionSteps);
			stepExists |= handle(stateSubject, stepIndex, m_spriteGenericAnimationSteps);
			
			break;
		case (ChapterNodeActionType::TEXT):
			stepExists |= handle(stateSubject, stepIndex, m_textRenderSteps);
			stepExists |= handle(stateSubject, stepIndex, m_textLineSteps);
			stepExists |= handle(stateSubject, stepIndex, m_textOverrideSpeakerSteps);
			stepExists |= handle(stateSubject, stepIndex, m_textOverrideColorSteps);

			break;
		case (ChapterNodeActionType::RELATION):
			//stepExists |= handleRelation(stateSubject, stepIndex, )

			break;
		case (ChapterNodeActionType::CHOICE):
			//std::function<void(ActionChoice&)> callback = stateSubject->m_choices.handle<ActionChoice>;
			stepExists |= handle(stateSubject, stepIndex, m_choiceTextOption);
			stepExists |= handle(stateSubject, stepIndex, m_choiceSetNextNode);
			stepExists |= handle(stateSubject, stepIndex, m_choiceRelationModifications);

			break;
		default:
			break;
		}
	}

	return stepExists;
	// Notify called in state after iter step is called
}


NodeState ChapterNode::action(StateSubject* stateSubject, int stepIndex) 
{
	// TODO: Remove m_temp
	std::cout << "test speaker" << " said: " << m_temp << std::endl;

	if (stepIndex < m_steps.size()) {
		bool stepDone = doStep(stateSubject, stepIndex);

		return NodeState::NODE_STEP;
	}

	std::cout << "step #" << stepIndex << std::endl;
	stateSubject->nodeEndActions();

	//std::cout << "step #" << stepIndex << " " << (stepDone ? "completed" : "not completed") << std::endl;

	return NodeState::NODE_END;
}
