#ifndef VN_STATE_DIALOGUE_H
#define VN_STATE_DIALOGUE_H

#include "state_types.h"
#include "chapter_node_types.h"
#include "model_subject.h"

class StateDialogue {
	// TODO add text style type for background translusent black for character thoughts
	// TextAction m_textAction{ TextAction::EMPTY };
private:
	TextState m_dialogueState{ false, L"", L"", glm::vec3() };

public:
	TextState& get() {
		return m_dialogueState;
	}

	void reset() {
		m_dialogueState = { false, L"", L"", glm::vec3() };
	}

public:
	//
	// Node interface
	//
	void handle(ActionTextLine& action) {
		m_dialogueState.m_line = action.m_line;

		Character* character = ModelSubject::getCharacterById(action.m_characterID);

		if (character != nullptr) {
			m_dialogueState.m_speakerName = character->getName();
			m_dialogueState.m_color = character->getTextColor();
		}
		else {
			std::cout << "handle ActionTextLine half failed" << std::endl;
		}
	}

	void handle(ActionTextOverrideSpeaker& action) {
		m_dialogueState.m_speakerName = action.m_speakerName;
	}

	void handle(ActionTextOverrideColor& action) {
		m_dialogueState.m_color = action.m_textColor;
	}

	void handle(ActionTextRender& action) {
		m_dialogueState.m_render = action.m_render;
	}
};

#endif // VN_STATE_DIALOGUE_H
