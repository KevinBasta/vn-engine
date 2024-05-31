

#include "chapter_node.h"
#include "state_subject.h"

NodeState ChapterNode::action(StateSubject* stateSubject, int stepIndex) 
{
	 //doStep(stateSubject, stepIndex);

	 stateSubject->updateCurrentText("test speaker", m_temp);

	 if (stepIndex == (m_steps.size() - 1)) {
		 /*if (m_children.size() > 1) {
			 return NodeState::NODE_CHOICE;
		 }
		 else {
			 return NodeState::NODE_END;
		 }*/
		return NodeState::NODE_END;
	 }
	 else {
		 return NodeState::NODE_STEP;
	 }
}