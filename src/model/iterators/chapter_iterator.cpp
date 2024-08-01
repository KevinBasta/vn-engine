
#include "chapter_iterator.h"

#include "model_subject.h"
#include "state_subject.h"

bool ChapterIterator::goToNextNode(StateSubject* stateSubject) {
	if (m_curNodePtr == nullptr || stateSubject == nullptr) {
		return false;
	}

	if (m_curNodePtr->getChildrenAmount() == 0) {
		// TODO: go to next chapter if there is one, otherwise return flase
		return false;
	}
	else {
		Node* child{ nullptr };
		id    childId{ 0 };

		if (stateSubject->m_choices.m_activeChoice) {
			childId = stateSubject->m_choices.getChoiceId();
			child = ModelSubject::getNodeById(childId);
			stateSubject->m_choices.recordAndDisableChoice();
		}

		if (child == nullptr) {
			// TODO: handle exceptions
			// TODO: should get any child not visited yet, with fall back to 0
			childId = m_curNodePtr->getFirstChildId();
			child = ModelSubject::getNodeById(childId);
		}

		if (child) {
			m_curNodeId = childId;
			m_curNodePtr = child;
			m_nodeRunner = m_curNodePtr->iter();

			return true;
		}
	}

	return false;
}