#ifndef VN_MODEL_ORCHESTRATOR_H
#define VN_MODEL_ORCHESTRATOR_H

#include "model_subject.h"
#include "state_subject.h"

#include "texture.h"

class SubjectsOrchestrator {
private:
	ModelSubject* m_model{};
	StateSubject* m_state{};
	Texture2D backgroundTexture; // TEMP

public:
	SubjectsOrchestrator(ModelSubject* model, StateSubject* state) :
		m_model{model},
		m_state{state},
		backgroundTexture{ "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\test.jpg" }
	{
		m_model->initCharacters();
	}

	void newGame() {
		m_model->createChapterOne();

		m_state->setTempTexture(m_model->m_characters[0].get()->getTexture(0));
		m_state->setTempBackground(&backgroundTexture);
	}
	
	void loadGame() {

	}

	void action() {
		m_state->action();
	}
};


#endif // VN_MODEL_ORCHESTRATOR_H
