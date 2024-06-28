#ifndef STATE_SUBJECT_H
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

private:
	// indicate that auto substeps (for animations) are present
	bool m_inSubStep{ false };

public:
	void setSubStep() {
		m_inSubStep = true;
	}

	void clearSubStep() {
		m_inSubStep = false;
	}

	void subAction() {
		iterator.subStep(this);
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
	typedef std::unordered_map<ID, SpriteState> spriteRenderMap;
	
	spriteRenderMap m_spriteRenderData{};

	spriteRenderMap& getSpriteRenderData() {
		return m_spriteRenderData;
	}

	void initCharacterData() {
		const ModelSubject::characterMap& characterMap = m_model->getCharacters();

		for (auto iter = characterMap.begin(); iter != characterMap.end(); iter++) {
			m_spriteRenderData[iter->first] = SpriteState{};
		}
	}

	void handle(ActionSpriteTexture& action) {
		try {
			SpriteState& state = m_spriteRenderData.at(action.m_characterID);
			
			// TODO: further null and err checks needed
			state.m_texture = m_model->getCharacterByID(action.m_characterID)->getTexture(action.m_textureIndex);
		}
		catch (std::out_of_range) {
			std::cerr << "ActionSpriteTexture::out_of_range" << std::endl;
		}

		m_stateDelta.push_back(StateDelta::SPRITE);
	}

	void handle(ActionSpriteOpacity& action) {
		try {
			SpriteState& state = m_spriteRenderData.at(action.m_characterID);
			state.m_position.m_opacity = action.m_opacity;
		}
		catch (std::out_of_range) {
			std::cerr << "ActionSpriteOpacity::out_of_range" << std::endl;
		}
		
		m_stateDelta.push_back(StateDelta::SPRITE);
	}

	void handle(ActionSpritePosition& action) {
		try {
			SpriteState& state = m_spriteRenderData.at(action.m_characterID);

			state.m_position.m_xCoord = action.m_xCoord;
			state.m_position.m_yCoord = action.m_yCoord;
			state.m_position.m_zCoord = action.m_zCoord;
			state.m_position.m_scale = action.m_scale;
		}
		catch (std::out_of_range) {
			std::cerr << "ActionSpritePosition::out_of_range" << std::endl;
		}

		m_stateDelta.push_back(StateDelta::SPRITE);
	}

	void handle(int characterID, ActionSpriteKeyframe& action) {

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
