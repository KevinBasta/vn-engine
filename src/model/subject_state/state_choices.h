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

public:
	StateChoices(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}
	~StateChoices() {}

	void reset() {
		m_currentChoiceIndex = 0;
		m_activeChoice = false;
		m_choices = nullptr;
		m_choiceSetNextNode = nullptr;
		m_choiceModifyRelations = nullptr;
		m_choiceSetNextChapter = nullptr;
	}

public:
	//
	// Iterator interface
	// 

	void applyChoice() {
		applySetNextNodeId();
		applyRelationModifications();
		applySetNextChapterId();

		reset();
	}

private:
	void applySetNextNodeId();
	void applyRelationModifications();
	void applySetNextChapterId();

public:
	//
	// View and Controller interface
	//
	void chooseUpChoice();
	void chooseDownChoice();

	bool isChoiceActive() { return m_activeChoice; }
	int getChoiceIndex() { return m_currentChoiceIndex; }
	ActionChoice* getChoices() { return m_choices; }

public:
	//
	// Node interface
	//
	void handle(ActionChoice& action) {
		m_activeChoice = true;
		m_currentChoiceIndex = 0;

		m_choices = &action;
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
};

#endif // VN_STATE_CHOICES_H
