

#include "chapter_node.h"
#include "state_subject.h"
#include "chapter_node_types.h"


void ChapterNode::doStep(StateSubject* stateSubject, int stepIndex) {
	// execture every action in current iter step
	std::cout << "STEP #########################" << stepIndex << std::endl;

	std::vector<ChapterNodeActionType>::iterator actionIter;
	for (actionIter = m_steps[stepIndex].begin(); actionIter < m_steps[stepIndex].end(); actionIter++) {
		std::cout << "ACTION ITER" << int(*actionIter) << std::endl;
		
		switch (*actionIter)
		{
		case ChapterNodeActionType::TYPE_TEXT:
		{
			std::vector<ChapterNodeText>::iterator iter;

			for (iter = m_textSteps[stepIndex].begin(); iter < m_textSteps[stepIndex].end(); iter++)
			{
				stateSubject->handle(*iter);
			}
			
			break;
		}
		case ChapterNodeActionType::CHANGE_SPRITE:
		{
			std::vector<ChapterNodeSprite>::iterator iter;

			for (iter = m_spriteSteps[stepIndex].begin(); iter < m_spriteSteps[stepIndex].end(); iter++)
			{
				std::cout << "sprite action!!!!!!!!!!!!!!!!!!" << std::endl;
				stateSubject->handle(*iter);
			}

			break;
		}
		case ChapterNodeActionType::CHANGE_BACKGROUND:
		{
			std::vector<ChapterNodeBackground>::iterator iter;

			for (iter = m_backgroundSteps[stepIndex].begin(); iter < m_backgroundSteps[stepIndex].end(); iter++)
			{
				stateSubject->handle(*iter);
			}

			break;
		}
		default:
			break;
		}
	}

	// notify called in state after iter step is called
}



NodeState ChapterNode::action(StateSubject* stateSubject, int stepIndex) 
{
	std::cout << "STEP ###########" << stepIndex << std::endl;
	stateSubject->updateCurrentText("test speaker", m_temp);
	if (stepIndex == (m_steps.size())) {
		/*if (m_children.size() > 1) {
			return NodeState::NODE_CHOICE;
		}
		else {
			return NodeState::NODE_END;
		}*/
		return NodeState::NODE_END;
	}
	else {
		doStep(stateSubject, stepIndex);
		return NodeState::NODE_STEP;
	}
}