#ifndef STATE_SUBJECT_H
#define STATE_SUBJECT_H

#include "subject.h"

#include "vn_engine.h"
#include "texture.h"
#include "state_types.h"
#include "chapter_node_types.h"

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

//std::unordered_map<Character&, CharacterSceneData> m_characterSceneData{};
// background variable can be defined here too

// Scene camera/projection data
/*
struct SceneCameraData {

};
*/


class StateSubject : public Subject {
private:
	ModelSubject* m_model{};
	Chapter* currentChapter{ nullptr };
	ChapterIterator iterator{ nullptr, 0 };
	
	// indicate that auto substeps (for animations) are present
	bool m_activeAutoAction{ false };

	// keeping record of what chioces were made for
	// safe file node traversal
	std::vector<int> chapterChoicesRecord{};


public:
	StateSubject(ModelSubject* modelSubject) : m_model{ modelSubject } {}
	~StateSubject() {}

	void initIterator(ChapterIterator chapterIterator) {
		iterator = chapterIterator;
	}

	void action() {
		if (inAutoAction()) {
			endAutoActions();
			clearAutoAction();
			notify();
			return;
		}

		iterator.step(this);

		// if state delta not empty
		notify();
		// then clear state delta
	}

	void setAutoAction() { m_activeAutoAction = true; }
	void clearAutoAction() { m_activeAutoAction = false; }
	bool inAutoAction() { return m_activeAutoAction; }

	void tickAutoActions(float timePassed);
	void endAutoActions();

	void appendChapterChoice(int choiceIndex) {
		chapterChoicesRecord.push_back(choiceIndex);
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

	// node ids for saving if needed
	// chapter numb, load chapter if not by asking model
	
	//
	// Text
	//
	// TextAction m_textAction{ TextAction::EMPTY };
	// TODO: can switch to string views if saved in model?
	TextState m_textState{TextAction::EMPTY, L"none", L"shirogane, 俺は", glm::vec3(81.0f / 255, 116.0f / 255, 150.0f / 255)};

	void handle(ActionTextLine& action) {
		
		Character* character = m_model->getCharacterByID(action.m_characterID);

		if (character != nullptr) {
			m_textState.m_currentState = TextAction::COMPLETE;
			m_textState.m_speakerName = character->getName();
			m_textState.m_line = action.m_line;
			m_textState.m_color = character->getTextColor();
		}
		else {
			std::cout << "handle ActionTextLine failed" << std::endl;
		}
		

		m_stateDelta.push_back(StateDelta::TEXT);
	}

	void handle(ActionTextOverrideSpeaker& action) {
		//m_textState.m_currentState = TextAction::COMPLETE;
		m_textState.m_speakerName = action.m_speakerName;
		
		m_stateDelta.push_back(StateDelta::TEXT);
	}

	void handle(ActionTextOverrideColor& action) {
		//m_textState.m_currentState = TextAction::COMPLETE;
		m_textState.m_color = action.m_textColor;
		
		m_stateDelta.push_back(StateDelta::TEXT);
	}

	//
	// Background
	//
	
	Texture2D* m_currentBackground{ nullptr };

	void updateCurrentBackground(Texture2D* newBackground) {
		m_currentBackground = newBackground;
	}

	void handle(ActionBackgroundTexture& backgroundAction) {
		// TODO: Error handling
		m_currentBackground = m_model->getBackgroundTexture(backgroundAction.backgroundIndex);

		m_stateDelta.push_back(StateDelta::BACKGROUND);
	}


	//
	// Characters
	//
	typedef int ID;
	typedef int stepIndex;
	typedef std::unordered_map<ID, SpriteState> spriteRenderMap;
	typedef std::list<std::pair<stepIndex, ActionSpriteAnimationGeneric>> activeSpriteAnimationsMap;
	
	spriteRenderMap m_spriteRenderData{};
	activeSpriteAnimationsMap m_activeSpriteAnimations{};
	spriteRenderMap& getSpriteRenderData() { return m_spriteRenderData; }
	void initCharacterData();

	void handle(ActionSpriteTexture& action);
	void handle(ActionSpriteOpacity& action);
	void handle(ActionSpritePosition& action);
	void handle(ActionSpriteAnimationGeneric& action);
	bool tickSpriteAnimations(float timePassed);
	bool tick(std::pair<stepIndex, ActionSpriteAnimationGeneric>& animation, float timePassed);
	bool endSpriteAnimations();

public:
	// Save specific state
	// character relationships
	// character items

public:
	// User input specific state
	//bool m_menuOpened{ false };


};

#endif // STATE_SUBJECT_H
