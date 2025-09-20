
#include "state_saves.h"
#include "state_subject.h"

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
	if (VNFSA::inSavesToLoad()) {
		return loadCurrentSave();
	}
	else if (VNFSA::inSavesToSave()) {
		return setCurrentSave();
	}

	return true;
}