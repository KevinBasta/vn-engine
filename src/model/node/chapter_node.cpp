

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
	
	stepExists |= handleStep(stateSubject, stepIndex, m_backgroundSteps);
	
	return stepExists;
	// Notify called in state after iter step is called
}



//
// Substep handling
// Bool return indicates if a substep occured 
//

bool ChapterNode::handeSubStepSpecialized(StateSubject* stateSubject, ActionSpriteAnimation& action, SubStepIndex subStepIndex) {

	bool hasSubStep{ false };

	if (stateSubject == nullptr) {
		return hasSubStep;
	}

	if (subStepIndex > 0 && subStepIndex < action.m_steps.size()) {
		hasSubStep = true;
		stateSubject->setSubStep();
		stateSubject->handle(action.m_characterID, action.m_steps[subStepIndex]);
	}

	return hasSubStep;
}


template <class T>
bool ChapterNode::handleSubStep(StateSubject* stateSubject, 
							    StepIndex stepIndex, 
								SubStepIndex subStepIndex, 
								std::unordered_map<StepIndex, std::vector<T>>& stepMap)
{
	bool hasSubStep{ false };

	if (stateSubject == nullptr) {
		return hasSubStep;
	}

	class std::unordered_map<StepIndex, std::vector<T>>::iterator stepLocation = stepMap.find(stepIndex);

	// Check if the step exsists for this type of action
	if (stepLocation != stepMap.end()) {
		class std::vector<T>::iterator action;

		// Execute all the actions within the step
		for (action = stepLocation->second.begin(); action < stepLocation->second.end(); action++)
		{
			bool subStepExecuted = handeSubStepSpecialized(stateSubject, *action, subStepIndex);

			if (subStepExecuted) {
				hasSubStep = true;
			}
		}
	}

	return hasSubStep;
}


bool ChapterNode::doSubStep(StateSubject* stateSubject, int stepIndex, int subStepIndex) {
	bool subStepExists{ false };

	if (stateSubject == nullptr) {
		return subStepExists;
	}

	subStepExists |= handleSubStep(stateSubject, stepIndex, subStepIndex, m_spriteAnimationSteps);

	return subStepExists;
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

NodeState ChapterNode::subAction(StateSubject* stateSubject, int stepIndex, int subStepIndex)
{
	std::cout << "sub action" << std::endl;
	bool subStepDone = doSubStep(stateSubject, stepIndex, subStepIndex);

	if (subStepDone) {
		return NodeState::NODE_SUBSTEP;
	}

	return NodeState::NODE_SUBSTEP_END;
}