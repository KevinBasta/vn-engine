
#include "state_main_menu.h"

#include "state_subject.h"

void StateMainMenu::applyCurrentChoice() {
	switch (m_mainMenuChoice) {
	case 0:
		m_stateSubject->newGame();
		break;
	case 1:
		m_stateSubject->goToSavesToLoad();
		break;
	case 2:

		break;
	case 3:
		StateSubject::VNFSA::gameState = StateSubject::VNFSA::VNState::QUIT;
		break;
	default:
		break;
	}
}