﻿#ifndef STATE_SUBJECT_H
#define STATE_SUBJECT_H

#include "subject.h"

#include "vn_engine.h"
#include "texture.h"
#include "state_delta.h"
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

public:
	StateSubject(ModelSubject* modelSubject) : m_model{ modelSubject } {}
	~StateSubject() {}

public:
	Chapter* currentChapter{ nullptr };
	ChapterIterator iterator{ nullptr, 0 };

	// keeping record of what chioces were made for
	// safe file node traversal
	std::vector<int> chapterChoicesRecord{};

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
	// TextAction m_textAction{ TextAction::EMPTY };
	// TODO: can switch to string views if saved in model?
	TextState m_textState{L"none", L"shirogane, 俺は", glm::vec3(81 / 255, 116 / 255, 150 / 255)};

	void handle(ActionTextLine& action) {
		
		Character* character = m_model->getCharacterByID(action.m_characterID);

		if (character != nullptr) {
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
		m_textState.m_speakerName = action.m_speakerName;
		
		m_stateDelta.push_back(StateDelta::TEXT);
	}

	void handle(ActionTextOverrideColor& action) {
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
	typedef int zIndex;
	typedef int ID;
	typedef std::unordered_map<ID, SpriteState> spriteRenderMap;
	
	spriteRenderMap m_spriteRenderData{};
	//std::unordered_map<zIndex, ChapterNodeSprite*> m_spriteIndexData{}; // can be vector by index?

	spriteRenderMap& getSpriteRenderData() {
		return m_spriteRenderData;
	}

	void initCharacterData() {
		const ModelSubject::characterMap& characterMap = m_model->getCharacters();

		for (auto iter = characterMap.begin(); iter != characterMap.end(); iter++) {
			m_spriteRenderData[iter->first] = SpriteState{};
		}
	}

	/*void setTempTexture(Texture2D* texture) {
		m_tempTexture = texture;
	}*/

	void handle(ActionSpriteTexture& action) {
		try {
			SpriteState& state = m_spriteRenderData[action.m_characterID];

			state.m_texture = m_model->getCharacterByID(action.m_characterID)->getTexture(action.m_textureIndex);
		}
		catch (...) {
			std::cout << "handle ActionSpriteTexture failed" << std::endl;
		}

		m_stateDelta.push_back(StateDelta::SPRITE);
	}

	void handle(ActionSpriteOpacity& action) {
		try {
			SpriteState& state = m_spriteRenderData[action.m_characterID];

			state.m_opacity = action.m_opacity;
		}
		catch (...) {
			std::cout << "handle ActionSpriteOpacity failed" << std::endl;
		}

		m_stateDelta.push_back(StateDelta::SPRITE);
	}

	void handle(ActionSpritePosition& action) {
		try {
			SpriteState& state = m_spriteRenderData[action.m_characterID];

			state.m_xCoord = action.m_xCoord;
			state.m_yCoord = action.m_yCoord;
			state.m_zCoord = action.m_zCoord;
			state.m_scale = action.m_scale;
		}
		catch (...) {
			std::cout << "handle ActionSpritePosition failed" << std::endl;
		}

		m_stateDelta.push_back(StateDelta::SPRITE);
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
