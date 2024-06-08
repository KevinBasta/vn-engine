#ifndef STATE_SUBJECT_H
#define STATE_SUBJECT_H

#include "subject.h"

#include "vn_engine.h"
#include "texture.h"
#include "state_delta.h"

#include "character.h"
//#include "chapter.h"
//#include "graph.h"
//#include "graph_iterator.h"
#include "chapter_iterator.h"
#include "model_subject.h"

#include <string>
#include <string_view>

#include <algorithm>
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



enum class TextState {
	EMPTY,
	TYPING,
	COMPLETE
};

class StateSubject : public Subject {
private:
	ModelSubject* m_model{};

public:
	StateSubject(ModelSubject* modelSubject) : m_model{ modelSubject } {}
	~StateSubject() {}

public:
	Chapter* currentChapter{ nullptr };
	ChapterIterator iterator{ nullptr, 0 };

	void initIterator(ChapterIterator chapterIterator) {
		iterator = chapterIterator;
	}

	void action() {
		iterator.step(this);

		// if state delta not empty
		notify();
		// then clear state delta
	}

public:
	// Delta tracking inquiry
	// an array of enums describing what changed to allow an
	// observer to fetch new data from only what changed
	
	std::vector<StateDelta> m_stateDelta{};

	void addStateDelta(StateDelta stateDelta) {
		m_stateDelta.push_back(stateDelta);
	}

	void clearStateDelta() {
		m_stateDelta.clear();
	}

	bool isInDelta(StateDelta stateDelta) {
		if (std::find(m_stateDelta.begin(), m_stateDelta.end(), stateDelta) != m_stateDelta.end()) {
			return true;
		}

		return false;
	}

public:
	// Model specific state
	// character textures and positions

	// node id for saving if needed
	// chapter numb, load chapter if not by asking model

	
	//
	// Text
	//

	TextState m_textState{ TextState::EMPTY };
	std::string m_currentSpeaker{};
	std::string m_currentText{};
	// can switch to string views if saved in model?

	void updateCurrentText(std::string newSpeaker, std::string newText) {
		m_currentSpeaker = newSpeaker;
		m_currentText = newText;
		m_textState = TextState::COMPLETE;

		//m_stateDelta.push_back(StateDelta::TEXT);

		std::cout << newSpeaker << " said: " << newText << std::endl;
	}

	void handle(ChapterNodeText& textAction) {


		m_stateDelta.push_back(StateDelta::TEXT);
	}


	//
	// Background
	//
	
	Texture2D* m_currentBackground{ nullptr };
	
	void setTempBackground(Texture2D* texture) {
		m_currentBackground = texture;
	}

	void updateCurrentBackground(Texture2D* newBackground) {
		m_currentBackground = newBackground;
	}

	void handle(ChapterNodeBackground& backgroundAction) {
		
		m_currentBackground = m_model->getBackgroundTexture(backgroundAction.backgroundIndex);

		m_stateDelta.push_back(StateDelta::BACKGROUND);
	}


	//
	// Characters
	//
	
	//Texture2D* m_tempTexture;

	/*void setTempTexture(Texture2D* texture) {
		m_tempTexture = texture;
	}*/

	void handle(ChapterNodeSprite& spriteAction) {


		//m_stateDelta.push_back(StateDelta::SPRITE);
	}

public:
	// Save specific state
	// character relationships
	// character items

public:
	// User input specific state
	//bool m_menuOpened{ false };


};

#endif // STATE_SUBJECT_H
