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
#include "state_next_node.h"
#include "state_next_chapter.h"

#include "character.h"
#include "relations.h"
//#include "chapter.h"
#include "chapter_iterator.h"
#include "model_subject.h"

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
			// that the next action isn't an autostep clearing action
			if (inAutoAction()) {
				endAutoActions();
				clearAutoAction();
				notify();
			}
		}	
	}

public:
	// non-serialize critical (partial data may be saved for savefile thumbnails for example)
	StateDialogue m_dialogue{};
	StateBackground m_background{};
	StateSprites m_sprites{ this };

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

		ChapterState result = iterator.step(this);

		std::cout << "ITERATOR RESULT: " << result << std::endl;

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
	void newGame() {
		m_relations.reset();
		
		m_dialogue.reset();
		m_sprites.reset();
		m_choices.reset();
		m_nextNode.reset();
		m_nextChapter.reset();
	}

	void loadSave() {

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
