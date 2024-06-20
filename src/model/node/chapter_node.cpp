

#include "chapter_node.h"
#include "state_subject.h"
#include "chapter_node_types.h"


template <class T>
void ChapterNode::handleStep(StateSubject *stateSubject, StepIndex stepIndex, std::unordered_map<StepIndex, std::vector<T>>& stepMap)
{
	class std::unordered_map<StepIndex, std::vector<T>>::iterator concreteSteps = stepMap.find(stepIndex);

	if (concreteSteps != stepMap.end()) {
		class std::vector<T>::iterator iter;

		for (iter = concreteSteps->second.begin(); iter < concreteSteps->second.end(); iter++)
		{
			stateSubject->handle(*iter);
		}
	}
}

void ChapterNode::doStep(StateSubject* stateSubject, int stepIndex) {
	// execture every action in current iter step

	// TODO: NEEDS EXCEPTION HANDLING FOR OUT OF RANGE m_steps, m_spriteSteps, etc..
	// TODO: CAN GET RID OF THE m_steps AND CHECK EVERY HASH MAP WITH HANDLESTEP
	std::vector<ChapterNodeActionType>::iterator actionIter;
	for (actionIter = m_steps[stepIndex].begin(); actionIter != m_steps[stepIndex].end(); actionIter++) {		
		switch (*actionIter)
		{
		case ChapterNodeActionType::TYPE_TEXT:
			handleStep(stateSubject, stepIndex, m_textSteps);
			break;
		case ChapterNodeActionType::CHANGE_SPRITE:
			handleStep(stateSubject, stepIndex, m_spriteTextureSteps);
			handleStep(stateSubject, stepIndex, m_spriteOpacitySteps);
			handleStep(stateSubject, stepIndex, m_spritePositionSteps);
			break;
		case ChapterNodeActionType::CHANGE_BACKGROUND:
			handleStep(stateSubject, stepIndex, m_backgroundSteps);
			break;
		default:
			break;
		}
	}

	// notify called in state after iter step is called
}

NodeState ChapterNode::action(StateSubject* stateSubject, int stepIndex) 
{
	// TODO: Remove
	stateSubject->updateCurrentText("test speaker", m_temp);

	if (stepIndex >= (m_steps.size())) {
		return NodeState::NODE_END;
	}
	else if (stepIndex == (m_steps.size() - 1)) {
		doStep(stateSubject, stepIndex);
		return NodeState::NODE_END;
	}
	else {
		doStep(stateSubject, stepIndex);
		return NodeState::NODE_STEP;
	}
}