
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