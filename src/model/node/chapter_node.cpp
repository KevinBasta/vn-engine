

#include "chapter_node.h"
#include "state_subject.h"
#include "chapter_node_types.h"

#include <list>
#include <vector>
#include <unordered_map>
#include <iterator>


//
// Step Handling
// Sends data to the subject
// Bool returns indicate if any step occured
//

template <class T>
bool ChapterNode::handleStep(StateSubject* stateSubject,
							 T& step)
{
	if (stateSubject == nullptr) {
		return false;
	}

	stateSubject->handle(step);

	return true;
}


template <class T>
bool ChapterNode::handleStep(StateSubject* stateSubject,
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

		stateSubject->handle(stepLocation->second);
	}

	return hasStep;
}

template <class T>
bool ChapterNode::handleStep(StateSubject *stateSubject,
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
			stateSubject->handle(*action);
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
			stepExists |= handleStep(stateSubject, stepIndex, m_backgroundSteps);

			break;
		case (ChapterNodeActionType::SPRITE):
			stepExists |= handleStep(stateSubject, stepIndex, m_spriteTextureSteps);
			stepExists |= handleStep(stateSubject, stepIndex, m_spriteOpacitySteps);
			stepExists |= handleStep(stateSubject, stepIndex, m_spritePositionSteps);
			stepExists |= handleStep(stateSubject, stepIndex, m_spriteGenericAnimationSteps);

			break;
		case (ChapterNodeActionType::TEXT):
			stepExists |= handleStep(stateSubject, stepIndex, m_textRenderSteps);
			stepExists |= handleStep(stateSubject, stepIndex, m_textLineSteps);
			stepExists |= handleStep(stateSubject, stepIndex, m_textOverrideSpeakerSteps);
			stepExists |= handleStep(stateSubject, stepIndex, m_textOverrideColorSteps);

			break;
		case (ChapterNodeActionType::CHOICE):
			stepExists |= handleStep(stateSubject, m_pickChildStep);
			
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
