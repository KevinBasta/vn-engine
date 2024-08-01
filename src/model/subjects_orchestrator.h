#ifndef VN_MODEL_ORCHESTRATOR_H
#define VN_MODEL_ORCHESTRATOR_H

#include "model_subject.h"
#include "state_subject.h"

#include "texture.h"

class SubjectsOrchestrator {
private:
	StateSubject* m_state{};

public:
	SubjectsOrchestrator(StateSubject* state) :
		m_state{state}
	{
		ModelSubject::initCharacters();
		ModelSubject::initBackgrounds();
		ModelSubject::initRelationTypes();
	}

	void newGame() {
		ModelSubject::createChapterOne();
		m_state->initIterator(ModelSubject::iter(0));
		m_state->newGame();
	}
	
	void loadGame() {

	}

	void action() {
		m_state->action();
	}
};


#endif // VN_MODEL_ORCHESTRATOR_H
