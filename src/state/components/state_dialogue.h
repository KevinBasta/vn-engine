#ifndef VN_STATE_DIALOGUE_H
#define VN_STATE_DIALOGUE_H

#include "state_types.h"
#include "chapter_node_types.h"
#include "model_subject.h"
#include "model_runtime_interface.h"

class StateDialogue {
	// TODO add text style type for background translusent black for character thoughts
	// TextAction m_textAction{ TextAction::EMPTY };
private:
	TextState m_dialogueState{ false, L"", L"", glm::vec3() };

public:
	TextState get() {
		return m_dialogueState;
	}

	void reset() {
		m_dialogueState = { false, L"", L"", glm::vec3() };
	}

public:
	//
	// Node interface
	//
	void handle(const ActionTextLine& action) {
		m_dialogueState.m_line = action.m_line;
		m_dialogueState.m_render = true;

		// TODO: should the character data be updatable? or just overriden by the actions?
		Character* character = ModelRuntimeInterface::getCharacterById(action.m_characterID);

		if (character != nullptr) {
			m_dialogueState.m_speakerName = character->getName();
			// TODO: text color override? Depends on order of actions. Make it order independant?
			m_dialogueState.m_color = character->getTextColor(); 
		}
		else {
			std::cout << "handle ActionTextLine half failed" << std::endl;
		}
	}

	void handle(const ActionTextOverrides& action) {
		if (action.m_renderEnabled) {
			m_dialogueState.m_render = action.m_render;
		}

		if (action.m_colorEnabled) {
			m_dialogueState.m_color = action.m_color;
		}

		if (action.m_speakerEnabled) {
			m_dialogueState.m_speakerName = action.m_speaker;
		}
	}
};

#endif // VN_STATE_DIALOGUE_H
