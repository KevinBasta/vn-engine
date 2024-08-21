#ifndef VN_MODEL_ORCHESTRATOR_H
#define VN_MODEL_ORCHESTRATOR_H

#include "model_subject.h"
#include "state_subject.h"

#include <memory>

class SubjectsOrchestrator {
private:
	std::unique_ptr<StateSubject> m_stateSubject{};

public:
	SubjectsOrchestrator() :
		m_stateSubject{ new StateSubject{} }
	{
		ModelSubject::init();
	}

	// Menu Options
	void newGame() {
		m_stateSubject.get()->newGame();
	}
	
	void loadGame() {

	}

	// Game runtime options
	void action() {
		m_stateSubject.get()->action();
	}

	// Other interface
	StateSubject* getState() {
		return m_stateSubject.get();
	}
};


#endif // VN_MODEL_ORCHESTRATOR_H
