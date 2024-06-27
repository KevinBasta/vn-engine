

#include "chapter_node.h"
#include "state_subject.h"
#include "chapter_node_types.h"


//
// Step Handling
// Sends data to the subject
// Bool returns indicate if any step occured
//

template <class T>
bool ChapterNode::handleStep(StateSubject *stateSubject, StepIndex stepIndex, std::unordered_map<StepIndex, std::vector<T>>& stepMap)
{
	bool hasStep{ false };

	if (stateSubject == nullptr) {
		return hasStep;
	}

	class std::unordered_map<StepIndex, std::vector<T>>::iterator concreteSteps = stepMap.find(stepIndex);

	if (concreteSteps != stepMap.end()) {
		hasStep = true;
		class std::vector<T>::iterator iter;

		for (iter = concreteSteps->second.begin(); iter < concreteSteps->second.end(); iter++)
		{
			stateSubject->handle(*iter);
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
	stepExists |= handleSubStep(stateSubject, stepIndex, 0, m_spriteAnimationSteps);
	
	stepExists |= handleStep(stateSubject, stepIndex, m_backgroundSteps);
	
	return stepExists;
	// Notify called in state after iter step is called
}



//
// Substep handling
// Bool return indicates if a substep occured 
//

template <class T>
bool ChapterNode::handleSubStep(StateSubject* stateSubject, StepIndex stepIndex, SubStepIndex subStepIndex, std::unordered_map<StepIndex, std::vector<T>>& stepMap)
{
	bool hasSubStep{ false };
	class std::unordered_map<StepIndex, std::vector<T>>::iterator concreteSteps = stepMap.find(stepIndex);

	if (concreteSteps != stepMap.end()) {
		class std::vector<T>::iterator iter;

		for (iter = concreteSteps->second.begin(); iter < concreteSteps->second.end(); iter++)
		{
			// auto itet
		}
	}

	return hasSubStep;
}


void ChapterNode::doSubStep(StateSubject* stateSubject, int stepIndex, int subStepIndex) {

}




NodeState ChapterNode::action(StateSubject* stateSubject, int stepIndex) 
{
	// TODO: Remove m_temp
	//stateSubject->updateCurrentText("test speaker", m_temp);
	std::cout << "test speaker" << " said: " << m_temp << std::endl;
	bool stepDone = doStep(stateSubject, stepIndex);
	std::cout << "step #" << stepIndex << std::endl;
	std::cout << "step done? " << stepDone << std::endl;

	if (stepDone) {
		return NodeState::NODE_STEP;
	}
	else {
		return NodeState::NODE_END;
	}
}

NodeState ChapterNode::subAction(StateSubject* stateSubject, int stepIndex, int subStepIndex)
{
	// TODO: Remove m_temp
	//stateSubject->updateCurrentText("test speaker", m_temp);
	std::cout << "sub action" << std::endl;

	return NodeState::NODE_END;
}