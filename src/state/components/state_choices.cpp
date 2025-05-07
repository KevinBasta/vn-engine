
#include "state_subject.h"
#include "state_choices.h"

void StateChoices::chooseUpChoice() {
	int choice{ m_currentChoiceIndex - 1 };

	if (choice >= 0 && choice < m_choices->choices.size()) {
		m_currentChoiceIndex = choice;
	}

	if (m_stateSubject != nullptr) {
		m_stateSubject->notify();
	}
}

void StateChoices::chooseDownChoice() {
	int choice{ m_currentChoiceIndex + 1 };

	if (choice >= 0 && choice < m_choices->choices.size()) {
		m_currentChoiceIndex = choice;
	}

	if (m_stateSubject != nullptr) {
		m_stateSubject->notify();
	}
}

void StateChoices::applySetNextNodeId() {
	if (m_choiceSetNextNode == nullptr) { return; }

	auto nodeId{
		std::find_if(m_choiceSetNextNode->nodeId.begin(), 
				  m_choiceSetNextNode->nodeId.end(), 
				  [current = m_currentChoiceIndex](auto& iter) { return (current == iter.first); })
	};

	if (nodeId != m_choiceSetNextNode->nodeId.end()) {
		m_stateSubject->m_nextNode.set(nodeId->second);
	}
}

void StateChoices::applySetNextChapterId() {
	if (m_choiceSetNextChapter == nullptr) { return; }

	auto chapterId{ 
		std::find_if(m_choiceSetNextChapter->chapterId.begin(), 
				  m_choiceSetNextChapter->chapterId.end(), 
				  [current = m_currentChoiceIndex](auto& iter) { return (current == iter.first); })
	};

	if (chapterId != m_choiceSetNextChapter->chapterId.end()) {
		m_stateSubject->m_nextChapter.set(chapterId->second);
	}	
}

void StateChoices::applyRelationModifications() {
	if (m_choiceModifyRelations == nullptr || m_stateSubject == nullptr) { return; }

	auto modificationsIter{
		std::find_if(m_choiceModifyRelations->relationModifications.begin(), 
				  m_choiceModifyRelations->relationModifications.end(), 
				  [current = m_currentChoiceIndex](auto& iter) { return (current == iter.first); })
	};

	if (modificationsIter != m_choiceModifyRelations->relationModifications.end()) {
		for (auto modification : modificationsIter->second) {
			m_stateSubject->m_relations.handle(modification);
		}
	}
}

