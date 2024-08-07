#ifndef STATE_SUBJECT_H
#define STATE_SUBJECT_H

#include "subject.h"

#include "vn_engine.h"
#include "texture.h"
#include "chapter_node_types.h"

#include "state_dialogue.h"
#include "state_sprites.h"
#include "state_background.h"
#include "state_relations.h"
#include "state_choices.h"
#include "state_types.h"

#include "character.h"
#include "relation.h"
//#include "chapter.h"
//#include "graph.h"
//#include "graph_iterator.h"
#include "chapter_iterator.h"
#include "model_subject.h"

#include <string>
#include <string_view>

#include <algorithm>
#include <vector>
#include <iostream>

//std::unordered_map<Character&, CharacterSceneData> m_characterSceneData{};
// background variable can be defined here too

// Scene camera/projection data
/*
struct SceneCameraData {

};
*/


class StateSubject : public Subject {
private:
	Chapter* currentChapter{ nullptr };
	ChapterIterator iterator{ nullptr, 0 };
	
public:
	// non-serialize critical (partial data may be saved for savefile thumbnails for example)
	StateDialogue m_dialogue{};
	StateBackground m_background{};
	StateSprites m_sprites{ this };

	// half-serialize critical (partial data needs to be saved)
	StateChoices m_choices{ this };

	// serialize critical (full data needs to be saved)
	StateRelations m_relations{};

	// Other states: camera, main menu, in-game menu

public:
	StateSubject() {}
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

		ChapterState result = iterator.step(this);

		std::cout << "ITERATOR RESULT: " << result << std::endl;

		// if state delta not empty
		notify();
		// then clear state delta
	}

	void nodeEndActions() {
		// clear non-presistent state
		m_dialogue.reset();
	}


	void newGame() {
		m_dialogue.reset();
		m_sprites.initCharacterData();
	}

	void loadSave() {

	}


public:
	// indicate that auto substeps (for animations) are present
	bool m_activeAutoAction{ false };

	bool inAutoAction() { return m_activeAutoAction; }
	void setAutoAction() { m_activeAutoAction = true; }
	void clearAutoAction() { m_activeAutoAction = false; }

	void tickAutoActions(float timePassed);
	void endAutoActions();


public:
	// Save specific state
	// character relationships
	// character items

public:
	// User input specific state
	//bool m_menuOpened{ false };

public:
	// TODO: remove, the entire scene/screen will have to be rerendred on any change anyways.
	// Perhaps it can be kept to avoid things like update on bond mutation only, but that seems rare
	// Delta tracking inquiry
	// an array of enums describing what changed to allow an
	// observer to fetch new data from only what changed
	// MODIFIED: to allow more optimized rendering descision

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

};

#endif // STATE_SUBJECT_H
