#ifndef STATE_SUBJECT_H
#define STATE_SUBJECT_H

#include "subject.h"

#include "vn_engine.h"
#include "texture.h"

#include "character.h"

#include <string>
#include <string_view>

#include <vector>

#define PLACEHOLDER_PATH "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\test.jpg"


//std::unordered_map<Character&, CharacterSceneData> m_characterSceneData{};
// background variable can be defined here too

// Scene camera/projection data
/*
struct SceneCameraData {

};
*/

struct CharacterSceneData {
	bool onScreen{ false };

	Texture2D* currentTexture;
	float scaleValue{ 1.0f };
	glm::mat4 model{ glm::mat4(1.0f) };

	CharacterSceneData(Character& character) :
		currentTexture{ character.getTexture(0) }
	{

	};
};


enum class StateDelta {
	TEXT,
	BACKGROUND
};

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
	
	std::vector<StateDelta> m_stateDelta{};

	std::vector<StateDelta>& getStateDelta() {
		return m_stateDelta;
	}

	void clearStateDelta() {
		m_stateDelta.clear();
	}

public:
	// Model specific state
	// character textures and positions

	// node id for saving if needed
	// chapter numb, load chapter if not by asking model



	// Background
	Texture2D* m_currentBackground;
	
	void setTempBackground(Texture2D* texture) {
		m_currentBackground = texture;
	}

	void updateCurrentBackground(Texture2D* newBackground) {
		m_currentBackground = newBackground;
		m_stateDelta.push_back(StateDelta::BACKGROUND);
		//notify();
	}

	// Character Text
	TextState m_textState{ TextState::EMPTY };
	std::string m_currentSpeaker{};
	std::string m_currentText{};
	// can switch to string views if saved in model?

	void updateCurrentText(std::string newSpeaker, std::string newText) {
		m_currentSpeaker = newSpeaker;
		m_currentText = newText;
		m_textState = TextState::COMPLETE;
		
		m_stateDelta.push_back(StateDelta::TEXT);
		
		std::cout << m_currentSpeaker << " said: " << m_currentText << std::endl;
		
		notify();
	}

public: 
	// current textures
	Texture2D* m_tempTexture;

	void setTempTexture(Texture2D* texture) {
		m_tempTexture = texture;
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
