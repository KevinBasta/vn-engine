
#include "chapter_iterator.h"

#include "node.h"
#include "chapter.h"

#include "model_subject.h"
#include "state_subject.h"

#include <iostream>

bool ChapterIterator::goToNextNode(StateSubject* stateSubject) {
	if (m_nodePtr == nullptr || m_chapterPtr == nullptr || stateSubject == nullptr) {
		return false;
	}

	if (m_nodePtr->getChildrenAmount() == 0) {
		std::cout << "CHILDREN 0" << std::endl;
		Chapter* chapter{ nullptr };
		id chapterId{ 0 };
		
		if (stateSubject->m_nextChapter.hasNextChapterId()) {
			// TODO: go to next chapter if there is one, otherwise return false
			chapterId = stateSubject->m_nextChapter.getNextChapterId();

			if (m_chapterPtr->hasChild(chapterId)) {
				chapter = ModelSubject::getChapterById(chapterId);
			}
		}

		if (chapter == nullptr) {
			try {
				chapterId = m_chapterPtr->getFirstChildId();
				chapter = ModelSubject::getChapterById(chapterId);
			}
			catch (...) {
				std::cout << "no more chapters lol" << std::endl;
			}
			
			std::cout << "chapter is not set by action" << std::endl;
			std::cout << chapterId << std::endl;
		}

		if (chapter) {
			std::cout << "chatper was resolved" << std::endl;

			if (m_chapterPtr->getChildrenAmount() > 1) {
				stateSubject->m_nextChapter.recordChapterChildChoice(chapterId);
			}

			m_chapterId.emplace(chapterId);
			updateChapterPtr();
			defaultNodeId();
			updateNodePtr();
			
			return true;
		}
		else {
			// TODO: go to main menu
		}
	}
	else {
		std::cout << "CHILDREN MORE THAN 0" << std::endl;
		Node* child{ nullptr };
		id    childId{ 0 };

		// Check if there is a chosen node
		if (stateSubject->m_nextNode.hasNextNodeId()) {
			childId = stateSubject->m_nextNode.getNextNodeId();

			if (m_nodePtr->hasChild(childId)) {
				child = ModelSubject::getNodeById(childId);
			}
		}

		// Get the first child otherwise 
		if (child == nullptr) {
			// TODO: handle exceptions
			// TODO: should get any child not visited yet, with fall back to 0
			try {
				childId = m_nodePtr->getFirstChildId();
				child = ModelSubject::getNodeById(childId);
			}
			catch (...) {
				std::cout << "no more nodes lol" << std::endl;
			}
			
			std::cout << "node is not set by action" << std::endl;
			std::cout << childId << std::endl;
		}

		if (child) {
			std::cout << "node was resolved" << std::endl;

			if (m_nodePtr->getChildrenAmount() > 1) {
				stateSubject->m_nextNode.recordNodeChildChoice(childId);
			}
			
			m_nodeId.emplace(childId);
			updateNodePtr();

			return true;
		}
	}

	return false;
}

void ChapterIterator::defaultChapterId() {
	m_chapterId.emplace(ModelSubject::getFirstChapterId());
}

void ChapterIterator::updateChapterPtr() {
	m_chapterPtr = ModelSubject::getChapterById(m_chapterId.value());

	if (m_chapterPtr == nullptr) {
		// TODO: create new exception class
		throw "bad";
	}
}

void ChapterIterator::defaultNodeId() {
	m_nodeId.emplace(m_chapterPtr->getHeadNodeId());
}

void ChapterIterator::updateNodePtr() {
	m_nodePtr = ModelSubject::getNodeById(m_nodeId.value());

	if (m_nodePtr == nullptr) {
		// TODO: create new exception class
		throw "bad";
	}

	m_nodeRunner = m_nodePtr->iter();
}

ChapterState ChapterIterator::step(StateSubject* stateSubject) {
	if (!m_chapterId.has_value()) { defaultChapterId(); }
	if (!m_chapterPtr)			  { updateChapterPtr(); }
	if (!m_nodeId.has_value())	  { defaultNodeId(); }
	if (!m_nodePtr)				  { updateNodePtr(); }

	if (stateSubject->m_choices.isChoiceActive()) {
		stateSubject->m_choices.applyChoice();
	}

	// TODO: abstract this to a different function so that when step is called again
	// can protect against inf recursion, aka TODO err checking for step(stateSubject);
	
	if (m_goToNextNode) {
		bool inNextNode{ goToNextNode(stateSubject) };

		if (!inNextNode) {
			return ChapterState::CHAPTERS_END;
		}

		m_goToNextNode = false;
		stateSubject->nodeEndActions();
	}

	NodeState state = m_nodeRunner.step(stateSubject);

	if (state == NodeState::NODE_END) {
		m_goToNextNode = true;
	}

	return ChapterState::CHAPTER_STEP;
}
