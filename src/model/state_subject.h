#ifndef STATE_SUBJECT_H
#define STATE_SUBJECT_H

#include "subject.h"

#include "vn_engine.h"
#include "texture.h"

#include <string>
#include <string_view>

#define PLACEHOLDER_PATH "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\placeholder.png"

enum class TextState {
	EMPTY,
	TYPING,
	COMPLETE
};

class StateSubject : Subject {
public:
	// Delta tracking inquiry
	// an array of enums describing what changed to allow an
	// observer to fetch new data from only what changed

public:
	// Model specific state
	TextState m_textState{ TextState::EMPTY };
	std::string m_currentSpeaker{};
	std::string m_currentText{};
	// can switch to string views if saved in model?

	Texture2D m_currentBackground{ PLACEHOLDER_PATH, TextureType::BACKGROUND};
	// character textures and positions

	void updateCurrentText(std::string newSpeaker, std::string newText) {
		m_currentSpeaker = newSpeaker;
		m_currentText = newText;
		m_textState = TextState::COMPLETE;
		notify();
	}

	void updateCurrentBackground(Texture2D newBackground) {
		m_currentBackground = newBackground;
		notify();
	}

public:
	// Save specific state
	// character relationships
	// character items

public:
	// User input specific state
	bool m_menuOpened{ false };


};

#endif // STATE_SUBJECT_H
