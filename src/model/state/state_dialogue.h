#ifndef VN_STATE_DIALOGUE_H
#define VN_STATE_DIALOGUE_H

#include "state_types.h"
#include "chapter_node_types.h"
#include "model_subject.h"

class StateDialogue {
	// TODO add text style type for background translusent black for character thoughts
	// TextAction m_textAction{ TextAction::EMPTY };
public:
	TextState m_dialogueState{ false, L"", L"", glm::vec3() };

	TextState& get() {
		return m_dialogueState;
	}

	void reset() {
		m_dialogueState = { false, L"", L"", glm::vec3() };
	}

	void handle(ActionTextLine& action) {

		m_dialogueState.m_line = action.m_line;

		Character* character = ModelSubject::getCharacterByID(action.m_characterID);

		if (character != nullptr) {
			m_dialogueState.m_speakerName = character->getName();
			m_dialogueState.m_color = character->getTextColor();
		}
		else {
			std::cout << "handle ActionTextLine half failed" << std::endl;
		}


		//m_stateDelta.push_back(StateDelta::TEXT);
	}

	void handle(ActionTextOverrideSpeaker& action) {
		//m_textState.m_currentState = TextAction::COMPLETE;
		m_dialogueState.m_speakerName = action.m_speakerName;

		//m_stateDelta.push_back(StateDelta::TEXT);
	}

	void handle(ActionTextOverrideColor& action) {
		//m_textState.m_currentState = TextAction::COMPLETE;
		m_dialogueState.m_color = action.m_textColor;

		//m_stateDelta.push_back(StateDelta::TEXT);
	}

	void handle(ActionTextRender& action) {
		m_dialogueState.m_render = action.m_render;

		//m_stateDelta.push_back(StateDelta::TEXT);
	}
};

#endif // VN_STATE_DIALOGUE_H
