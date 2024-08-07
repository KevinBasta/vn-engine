
#include "chapter_iterator.h"

#include "model_subject.h"
#include "state_subject.h"

#include <iostream>

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
		
		if (stateSubject->m_choices.hasNextNodeId()) {
			childId = stateSubject->m_choices.getChoiceNodeId();
			child = ModelSubject::getNodeById(childId);
		}

		if (child == nullptr) {
			// TODO: handle exceptions
			// TODO: should get any child not visited yet, with fall back to 0
			childId = m_curNodePtr->getFirstChildId();
			child = ModelSubject::getNodeById(childId);
		}

		if (child) {
			if (m_curNodePtr->getChildrenAmount() > 1) {
				stateSubject->m_choices.recordNodeChildChoice(childId);
			}
			
			m_curNodeId = childId;
			m_curNodePtr = child;
			m_nodeRunner = m_curNodePtr->iter();

			return true;
		}
	}

	return false;
}

ChapterState ChapterIterator::step(StateSubject* stateSubject) {
	if (m_curChapterPtr == nullptr) {
		m_curChapterPtr = ModelSubject::getChapterById(m_curChapterId);
		if (m_curChapterPtr == nullptr) {
			// TODO: throw exception instead??
			return ChapterState::CHAPTER_ERR;
		}
	}

	if (m_curNodePtr == nullptr) {
		m_curNodeId = m_curChapterPtr->getHeadNodeId();
		m_curNodePtr = ModelSubject::getNodeById(m_curNodeId);

		if (m_curNodePtr == nullptr) {
			// TODO: throw exception instead??
			return ChapterState::CHAPTER_ERR;
		}

		m_nodeRunner = m_curNodePtr->iter();
	}

	if (stateSubject->m_choices.isChoiceActive()) {
		stateSubject->m_choices.disableChoice();
	}

	// TODO: abstract this to a different function so that when step is called again
	// can protect against inf recursion, aka TODO err checking for step(stateSubject);
	NodeState state = m_nodeRunner.step(stateSubject);

	if (state == NodeState::NODE_END) {
		bool inNextNode{ goToNextNode(stateSubject) };
		if (!inNextNode) {
			return ChapterState::CHAPTERS_END;
		}
		else {
			// TODO: check return?
			step(stateSubject);
		}
	}

	return ChapterState::CHAPTER_STEP;
}
