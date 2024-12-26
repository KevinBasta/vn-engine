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
	ActionChoice const* m_choices{ nullptr };
	ActionChoiceSetNextNode const* m_choiceSetNextNode{ nullptr };
	ActionChoiceModifyRelation const* m_choiceModifyRelations{ nullptr };
	ActionChoiceSetNextChapter const* m_choiceSetNextChapter{ nullptr };

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
	const ActionChoice* getChoices() { return m_choices; }

public:
	//
	// Node interface
	//
	void handle(const ActionChoice& action) {
		m_activeChoice = true;
		m_currentChoiceIndex = 0;

		m_choices = &action;
	}

	void handle(const ActionChoiceSetNextNode& action) {
		std::cout << "pick next node based on choice" << std::endl;
		m_choiceSetNextNode = &action;
	}
	
	void handle(const ActionChoiceModifyRelation& action) {
		std::cout << "modify relations based on choice" << std::endl;
		m_choiceModifyRelations = &action;
	}

	void handle(const ActionChoiceSetNextChapter& action) {
		std::cout << "pick next chapter based on choice" << std::endl;
		m_choiceSetNextChapter = &action;
	}
};

#endif // VN_STATE_CHOICES_H
