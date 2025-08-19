#ifndef VN_STATE_MAIN_MENU_H
#define VN_STATE_MAIN_MENU_H

#include "id.h"
#include "node_types.h"
#include "model_subject.h"

#include <optional>
#include <vector>

class StateMainMenu {
private:
	StateSubject* m_stateSubject{ nullptr };
	
	int m_mainMenuChoice{ 0 };
	const int m_mainMenuMaxChoice{ 3 };

public:
	StateMainMenu(StateSubject* stateSubject) :
		m_stateSubject{ stateSubject }
	{

	}

	void applyMainMenuChoice();

	int currentChoice() {
		return m_mainMenuChoice;
	}

	void chooseUpChoice() {
		if (m_mainMenuChoice <= 0) {
			m_mainMenuChoice = 0;
			return;
		}

		m_mainMenuChoice--;
	}
	void chooseDownChoice() {
		if (m_mainMenuChoice >= 3) {
			m_mainMenuChoice = 3;
			return;
		}

		m_mainMenuChoice++;
	}

	void reset() {
		m_mainMenuChoice = 0;
	}
};


#endif // VN_STATE_MAIN_MENU_H