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
	void handle(const ActionTextLine& action) {
		m_dialogueState.m_line = action.m_line;

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

	void handle(const ActionTextOverrideSpeaker& action) {
		m_dialogueState.m_speakerName = action.m_speakerName;
	}

	void handle(const ActionTextOverrideColor& action) {
		m_dialogueState.m_color = action.m_textColor;
	}

	void handle(const ActionTextRender& action) {
		m_dialogueState.m_render = action.m_render;
	}
};

#endif // VN_STATE_DIALOGUE_H
