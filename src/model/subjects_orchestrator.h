#ifndef VN_MODEL_ORCHESTRATOR_H
#define VN_MODEL_ORCHESTRATOR_H

#include "model_subject.h"
#include "state_subject.h"

#include "texture.h"

class SubjectsOrchestrator {
private:
	ModelSubject* m_model{};
	StateSubject* m_state{};

public:
	SubjectsOrchestrator(ModelSubject* model, StateSubject* state) :
		m_model{model},
		m_state{state}
	{
		m_model->initCharacters();
		m_model->initBackgrounds();
		m_model->initRelationTypes();
	}

	void newGame() {
		m_model->createChapterOne();

		m_state->initIterator(m_model->iter(0));
		m_state->initCharacterData();
	}
	
	void loadGame() {

	}

	void action() {
		m_state->action();
	}
};


#endif // VN_MODEL_ORCHESTRATOR_H
