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


public:
	// indicate that auto substeps (for animations) are present
	bool m_activeAutoAction{ false };

	void setAutoAction() { m_activeAutoAction = true; }
	void clearAutoAction() { m_activeAutoAction = false; }
	bool inAutoAction() { return m_activeAutoAction; }

	void tickAutoActions(float timePassed);
	void endAutoActions();


public:
	// keeping record of what chioces were made for
	// safe file node traversal
	bool m_activeChoice{ false };
	ActionChooseNode* m_nodeChoices{ nullptr };
	int m_currentChoiceIndex{ 0 };

	std::vector<int> m_chapterChoicesRecord{};

	bool isChoiceActive() {
		return m_activeChoice;
	}

	void appendChapterChoice() {
		m_chapterChoicesRecord.push_back(getChoiceId());
	}

	ActionChooseNode* getNodeChoices() {
		return m_nodeChoices;
	}

	void chooseNode(int nodeId) {

	}

	void chooseUpChoice() {
		int choice{ m_currentChoiceIndex - 1 };

		if (choice >= 0 && choice < m_nodeChoices->m_choices.size()) {
			m_currentChoiceIndex = choice;
		}
	
		notify();
	}

	void chooseDownChoice() {
		int choice{ m_currentChoiceIndex + 1 };

		if (choice >= 0 && choice < m_nodeChoices->m_choices.size()) {
			m_currentChoiceIndex = choice;
		}

		notify();
	}

	int getChoiceId() {
		if (m_activeChoice == false || m_nodeChoices == nullptr) {
			return 0;
		}

		if (m_currentChoiceIndex < 0 || m_currentChoiceIndex >= m_nodeChoices->m_choices.size()) {
			return 0;
		}

		return m_nodeChoices->m_choices[m_currentChoiceIndex].m_nodeID;
	}

	void recordAndDisableChoice() {
		appendChapterChoice();
		m_activeChoice = false;
		m_nodeChoices = nullptr;
		m_currentChoiceIndex = 0;
	}

	void handle(ActionChooseNode& action) {
		m_activeChoice = true;

		m_nodeChoices = &action;
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
	TextState m_textState{false, L"", L"", glm::vec3()};

	void resetTextState() {
		m_textState = { false, L"", L"", glm::vec3() };
	}

	void handle(ActionTextLine& action) {
		
		m_textState.m_line = action.m_line;
		
		Character* character = m_model->getCharacterByID(action.m_characterID);

		if (character != nullptr) {
			m_textState.m_speakerName = character->getName();
			m_textState.m_color = character->getTextColor();
		}
		else {
			std::cout << "handle ActionTextLine half failed" << std::endl;
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

	void handle(ActionTextRender& action) {
		m_textState.m_render = action.m_render;

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
	// clear non-presistent state
	void nodeEndActions() {
		resetTextState();
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
