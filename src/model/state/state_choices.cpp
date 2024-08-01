
#include "state_subject.h"
#include "state_choices.h"

void StateChoices::chooseUpChoice() {
	int choice{ m_currentChoiceIndex - 1 };

	if (choice >= 0 && choice < m_nodeChoices->m_choices.size()) {
		m_currentChoiceIndex = choice;
		updateChoiceId();
	}

	//TODO: null checks
	m_stateSubject->notify();
}

void StateChoices::chooseDownChoice() {
	int choice{ m_currentChoiceIndex + 1 };

	if (choice >= 0 && choice < m_nodeChoices->m_choices.size()) {
		m_currentChoiceIndex = choice;
		updateChoiceId();
	}

	m_stateSubject->notify();
}