

#include "chapter_node.h"
#include "state_subject.h"
#include "chapter_node_types.h"


//
// Step Handling
// Sends data to the subject
// Bool returns indicate if any step occured
//

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
		return stepExists;
	}

	stepExists |= handleStep(stateSubject, stepIndex, m_textLineSteps);
	stepExists |= handleStep(stateSubject, stepIndex, m_textOverrideSpeakerSteps);
	stepExists |= handleStep(stateSubject, stepIndex, m_textOverrideColorSteps);

	stepExists |= handleStep(stateSubject, stepIndex, m_spriteTextureSteps);
	stepExists |= handleStep(stateSubject, stepIndex, m_spriteOpacitySteps);
	stepExists |= handleStep(stateSubject, stepIndex, m_spritePositionSteps);
	stepExists |= handleStep(stateSubject, stepIndex, m_spriteAnimationSteps);
	//stepExists |= handleStep(stateSubject, stepIndex, m_spriteGenericAnimationSteps);
	
	stepExists |= handleStep(stateSubject, stepIndex, m_backgroundSteps);
	
	return stepExists;
	// Notify called in state after iter step is called
}


NodeState ChapterNode::action(StateSubject* stateSubject, int stepIndex) 
{
	// TODO: Remove m_temp
	std::cout << "test speaker" << " said: " << m_temp << std::endl;
	bool stepDone = doStep(stateSubject, stepIndex);
	std::cout << "step #" << stepIndex << " " << (stepDone ? "completed" : "not completed") << std::endl;

	if (stepDone) {
		return NodeState::NODE_STEP;
	}

	return NodeState::NODE_END;
}
