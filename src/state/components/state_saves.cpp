
#include "state_saves.h"
#include "state_subject.h"

bool StateSaves::handleEscape() {
	if (m_stateSubject->inSavesToLoad()) {
		m_stateSubject->goToMainMenu();
	}
	else if (m_stateSubject->inSavesToSave()) {
		m_stateSubject->goToInGameWithSideBar();
	}

	return true;
}

bool StateSaves::loadCurrentSave() {
	m_stateSubject->loadSave(/*pass the save index or obj*/);

	return true;
}

bool StateSaves::setCurrentSave() {
	// Delete save in current spot if exsists

	// Save the current state

	return true;
}

bool StateSaves::applyCurrentChoice() {
	if (m_stateSubject->inSavesToLoad()) {
		return loadCurrentSave();
	}
	else if (m_stateSubject->inSavesToSave()) {
		return setCurrentSave();
	}

	return true;
}