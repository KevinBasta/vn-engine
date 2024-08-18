#ifndef VN_STATE_CHOICES_H
#define VN_STATE_CHOICES_H

#include "id.h"
#include "chapter_node_types.h"

#include <vector>

class StateSubject;

class StateChoices {
private:
	StateSubject* m_stateSubject{ nullptr };

	bool m_activeChoice{ false };
	ActionChoice* m_choices{ nullptr };
	ActionChoiceSetNextNode* m_choiceSetNextNode{ nullptr };
	ActionChoiceModifyRelation* m_choiceModifyRelations{ nullptr };
	ActionChoiceSetNextChapter* m_choiceSetNextChapter{ nullptr };

	int m_currentChoiceIndex{ 0 };

	bool m_nextNodeIdSet{ false };
	id m_nextNodeId{ 0 };

private:
	// Serializable
	bool m_nextChapterIdSet{ false };
	id m_nextChapterId{ 0 };

public:
	void setNextNodeId(id nextNodeId) {
		m_nextNodeIdSet = true;
		m_nextNodeId = nextNodeId;
	}

public:
	StateChoices(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}
	
private:
	//
	// Keep record of what choices were made for save file
	//
	std::vector<int> m_chapterChoicesRecord{};

public:
	//
	// Iterator interface
	// 

	bool hasNextNodeId() {
		return m_nextNodeIdSet;
	}

	id getChoiceNodeId() {
		return m_nextNodeId;
	}

	void consumeChoice() {
		applySetNextNodeId();
		applyRelationModifications();
		applySetNextChapterId();

		m_activeChoice = false;
		m_choices = nullptr;
		m_choiceSetNextNode = nullptr;
		m_choiceModifyRelations = nullptr;
		m_choiceSetNextChapter = nullptr;
	}

	void recordNodeChildChoice(id nodeId) {
		m_chapterChoicesRecord.push_back(nodeId);
	}

private:
	//
	// Helpers for iterator interface
	// 
	void applySetNextNodeId();
	void applyRelationModifications();
	void applySetNextChapterId();

public:
	//
	// View and Controller interface
	//
	void chooseUpChoice();
	void chooseDownChoice();

	bool isChoiceActive() {
		return m_activeChoice;
	}

	int getChoiceIndex() {
		return m_currentChoiceIndex;
	}

	ActionChoice* getChoices() {
		return m_choices;
	}

public:
	//
	// Node interface
	//
	void handle(ActionChoice& action) {
		m_activeChoice = true;

		m_choices = &action;

		m_currentChoiceIndex = 0;
		m_nextNodeIdSet = false;
		m_nextNodeId = 0;
	}

	void handle(ActionChoiceSetNextNode& action) {
		std::cout << "pick next node based on choice" << std::endl;
		m_choiceSetNextNode = &action;
	}
	
	void handle(ActionChoiceModifyRelation& action) {
		std::cout << "modify relations based on choice" << std::endl;
		m_choiceModifyRelations = &action;
	}

	void handle(ActionChoiceSetNextChapter& action) {
		std::cout << "pick next chapter based on choice" << std::endl;
		m_choiceSetNextChapter = &action;
	}

	void handle(ActionSetNextChapter& action) {
		std::cout << "set next chapter" << std::endl;
		m_nextChapterIdSet = true;
		m_nextChapterId = action.m_chapterId;
	}

	void setNextChapterId(id chapterId) {

	}
};

#endif // VN_STATE_CHOICES_H
