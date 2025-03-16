
#include "state_subject.h"
#include "state_choices.h"

void StateChoices::chooseUpChoice() {
	int choice{ m_currentChoiceIndex - 1 };

	if (choice >= 0 && choice < m_choices->m_choices.size()) {
		m_currentChoiceIndex = choice;
	}

	if (m_stateSubject != nullptr) {
		m_stateSubject->notify();
	}
}

void StateChoices::chooseDownChoice() {
	int choice{ m_currentChoiceIndex + 1 };

	if (choice >= 0 && choice < m_choices->m_choices.size()) {
		m_currentChoiceIndex = choice;
	}

	if (m_stateSubject != nullptr) {
		m_stateSubject->notify();
	}
}

void StateChoices::applySetNextNodeId() {
	if (m_choiceSetNextNode == nullptr) { return; }

	auto nodeId{ m_choiceSetNextNode->m_nodeId.find(m_currentChoiceIndex) };

	if (nodeId != m_choiceSetNextNode->m_nodeId.end()) {
		m_stateSubject->m_nextNode.set(nodeId->second);
	}
}

void StateChoices::applySetNextChapterId() {
	if (m_choiceSetNextChapter == nullptr) { return; }

	auto chapterId{ m_choiceSetNextChapter->m_chapterId.find(m_currentChoiceIndex) };

	if (chapterId != m_choiceSetNextChapter->m_chapterId.end()) {
		m_stateSubject->m_nextChapter.set(chapterId->second);
	}	
}

void StateChoices::applyRelationModifications() {
	if (m_choiceModifyRelations == nullptr || m_stateSubject == nullptr) { return; }

	auto modificationsIter{ m_choiceModifyRelations->m_relationModifications.find(m_currentChoiceIndex) };

	if (modificationsIter != m_choiceModifyRelations->m_relationModifications.end()) {
		for (auto modification : modificationsIter->second) {
			m_stateSubject->m_relations.handle(modification);
		}
	}
}

