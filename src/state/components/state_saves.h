#ifndef VN_STATE_SAVES_H
#define VN_STATE_SAVES_H

#include "id.h"
#include "node_types.h"
#include "model_subject.h"

class StateSaves {
private:
	StateSubject* m_stateSubject{ nullptr };

	int m_savesChoice{ 0 };
	const int m_savesMaxChoice{ 99 };

public:
	StateSaves(StateSubject* stateSubject) :
		m_stateSubject{ stateSubject }
	{

	}

	bool handleEscape();
	bool loadCurrentSave();
	bool setCurrentSave();
	bool applyCurrentChoice();

	int currentChoice() {
		return m_savesChoice;
	}

	void chooseUpChoice() {
		if (m_savesChoice <= 0) {
			m_savesChoice = 0;
			return;
		}

		m_savesChoice--;
	}
	void chooseDownChoice() {
		if (m_savesChoice >= m_savesMaxChoice) {
			m_savesChoice = m_savesMaxChoice;
			return;
		}

		m_savesChoice++;
	}

	void reset() {
		m_savesChoice = 0;
	}
};


#endif // VN_STATE_SAVES_H