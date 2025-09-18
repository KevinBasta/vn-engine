#ifndef STATE_SUBJECT_H
#define STATE_SUBJECT_H

#include "subject.h"

#include "vn_engine.h"
#include "texture.h"
#include "node_types.h"

#include "state_FSA.h"

#include "state_dialogue.h"
#include "state_sprites.h"
#include "state_background.h"
#include "state_relations.h"
#include "state_choices.h"
#include "state_types.h"
#include "state_next_node.h"
#include "state_next_chapter.h"
#include "state_main_menu.h"
#include "state_saves.h"

#include "character.h"
#include "relations.h"
//#include "chapter.h"
#include "chapter_iterator.h"

#include <string>
#include <string_view>

#include <algorithm>
#include <vector>
#include <iostream>

class StateSubject : public Subject {
private:
	Chapter* currentChapter{ nullptr };
	ChapterIterator iterator{};
	
public:
	// Engine operations
	id getChapterId() { return iterator.getChapterId(); }
	id getNodeId() { return iterator.getNodeId(); }
	int getStepIndex() { return iterator.getCurrentStepIndex(); }

	void goToChapterId(id chapterId) { chapterEndActions(); iterator = ChapterIterator{ chapterId }; }
	void goToNodeId(id nodeId) { nodeEndActions(); iterator = ChapterIterator{ iterator.getChapterId(), nodeId }; }
	
	void goToStepIndex(index stepIndex) { 
		// First clear any autoactions from current iterator
		// So that an action to get to stepIndex is not spent on clearing
		// the auto action
		if (inAutoAction()) {
			endAutoActions();
			clearAutoAction();
			notify();
		}

		// Next get the current chapter and node, then set the iterator to them
		id chapterId{ getChapterId() };
		id nodeId{ getNodeId() };
		goToChapterId(chapterId);
		goToNodeId(nodeId);

		for (index i{ 0 }; i <= stepIndex; i++) {
			// Do each action up to and including stepIndex
			action();
		
			// If autoactions are set by the action, clear them so 
			// that the next action (click) isn't an autostep clearing action
			// TODO: i != stepIndex allows for the autoactions at the target step
			// to play. Perhaps can be only for a special case where the engine modifies the autoaction
			if (/*i != stepIndex && */inAutoAction()) {
				endAutoActions();
				clearAutoAction();
				notify();
			} 
			// TODO: MAYBE IF EDITING ANIMATIONS IT WOULD BE HELPFUL TO NOT CLEAR THEM
			// IN THE CASE THAT i == stepIndex. Maybe should be an option in ui? or default behaviour?
		}	
	}

public:
	// non-serialize critical (partial data may be saved for savefile thumbnails for example)
	StateDialogue m_dialogue{};
	StateBackground m_background{};
	StateSprites m_sprites{ this };

	StateMainMenu m_mainMenu{ this };
	StateSaves m_savesMenu{ this };

	// half-serialize critical (partial data needs to be saved)
	StateChoices m_choices{ this };
	StateNextNode m_nextNode{};
	StateNextChapter m_nextChapter{};


	// serialize critical (full data needs to be saved)
	StateRelations m_relations{ this };

	// Other states: camera/projection data, main menu, in-game menu, character items

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

		try {
			ChapterState result = iterator.step(this);
			
			std::cout << "ITERATOR RESULT: " << result << std::endl;
		}
		catch (...) {
			chapterEndActions();
			goToMainMenu();
			return;
		}


		// if state delta not empty
		notify();
		// then clear state delta
	}

	void nodeEndActions() {
		// clear non-presistent state
		m_dialogue.reset();
		m_sprites.reset();
		m_choices.reset();
		m_nextNode.reset();
	}

	void chapterEndActions() {
		m_dialogue.reset();
		m_sprites.reset();
		m_choices.reset();
		m_nextNode.reset();
		m_nextChapter.reset();
	}


public:
	//
	// Menu operations
	//
	void quitGame() {
		VNFSA::transition(VNState::QUIT);
	}

	void goToMainMenu() {
		VNFSA::transition(VNState::MAIN_MENU);

	}

	void goToSavesToLoad() {
		VNFSA::transition(VNState::SAVES_MENU_LOAD);
		
	}

	void goToOptionsMainMenu() {
		VNFSA::transition(VNState::OPTIONS_MENU_MAIN_MENU);
		
	}

	void goToSavesToSave() {
		VNFSA::transition(VNState::SAVES_MENU_SAVE);

	}

	void goToInGameWithSideBar() {
		VNFSA::transition(VNState::IN_GAME_WITH_SIDE_BAR);
	}

	void goToInGame() {
		VNFSA::transition(VNState::IN_GAME);
	}

	void newGame() {
		VNFSA::transition(VNState::IN_GAME);

		m_relations.reset();
		
		m_dialogue.reset();
		m_sprites.reset();
		m_choices.reset();
		m_nextNode.reset();
		m_nextChapter.reset();

		m_mainMenu.reset();

		// Go to first node
		action();
	}

	void loadSave() {
		VNFSA::transition(VNState::IN_GAME);

	}

private: 
	//
	// Indicate that auto substeps (for animations) are present
	//
	bool m_activeAutoAction{ false };

public:
	// Interface for model and view
	bool inAutoAction() { return m_activeAutoAction; }
	void setAutoAction() { m_activeAutoAction = true; }
	void clearAutoAction() { m_activeAutoAction = false; }

	// Interface for view
	void tickAutoActions(float timePassed);
	void endAutoActions();



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
