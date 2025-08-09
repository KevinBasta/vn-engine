#ifndef VN_STATE_DIALOGUE_H
#define VN_STATE_DIALOGUE_H

#include "state_types.h"
#include "node_types.h"
#include "model_subject.h"
#include "model_runtime_interface.h"

class StateDialogue {
	// TODO add text style type for background translusent black for character thoughts
	// TextAction m_textAction{ TextAction::EMPTY };
private:
	TextState m_dialogueState{ false, false, L"", false, L"", glm::vec3() };

public:
	TextState get() {
		return m_dialogueState;
	}

	void reset() {
		m_dialogueState = { false, false, L"", false, L"", glm::vec3() };
	}

public:
	//
	// Node interface
	//
	void handle(const ActionTextLine& action) {
		m_dialogueState.render = true;
		m_dialogueState.line = action.line;

		if (action.narration) {
			m_dialogueState.speakerEnabled = false;
			m_dialogueState.speakerName = L"";
			m_dialogueState.narrationBackground = true;
			m_dialogueState.color = { 1.0f, 1.0f, 1.0f };
		}
		else {
			// TODO: should the character data be updatable? or just overriden by the actions?
			Character* character = ModelRuntimeInterface::getCharacterById(action.characterId);

			if (character != nullptr) {
				m_dialogueState.speakerEnabled = true;
				m_dialogueState.speakerName = character->getName();
				m_dialogueState.narrationBackground = false;
				m_dialogueState.color = character->getTextColor(); // TODO: text color override? Depends on order of actions. Make it order independant?
			}
			else {
				m_dialogueState = { false, false, L"", false, L"", glm::vec3() };
				std::cout << "handle ActionTextLine half failed" << std::endl;
			}
		}
	}

	void handle(const ActionTextOverrides& action) {
		if (action.renderEnabled) {
			m_dialogueState.render = action.render;
		}

		if (action.colorEnabled) {
			m_dialogueState.color = action.color;
		}

		if (action.speakerEnabled) {
			m_dialogueState.speakerName = action.speaker;
		}
	}
};

#endif // VN_STATE_DIALOGUE_H
