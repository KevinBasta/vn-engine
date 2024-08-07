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
	ActionChoiceSetNextNode* m_choiceNextNode{ nullptr };
	ActionChoiceModifyRelation* m_choiceModifyRelation{ nullptr };

	int m_currentChoiceIndex{ 0 };

	bool m_nextNodeIdSet{ false };
	id m_nextNodeId{ 0 };

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

	void disableChoice() {
		saveNextNodeId();
		m_activeChoice = false;

		m_choices = nullptr;
		m_choiceNextNode = nullptr;
		m_choiceModifyRelation = nullptr;
	}

	void recordNodeChildChoice(id nodeId) {
		m_chapterChoicesRecord.push_back(nodeId);
	}

private:
	//
	// Iterator interface helpers
	// 
	void saveNextNodeId() {
		if (m_choiceNextNode == nullptr) { return; }

		auto nodeId{ m_choiceNextNode->m_nodeId.find(m_currentChoiceIndex) };

		if (nodeId != m_choiceNextNode->m_nodeId.end()) {
			m_nextNodeIdSet = true;
			m_nextNodeId = nodeId->second;
		}
	}

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
		std::cout << "NEXT NODE CHOICE ACTION SET" << std::endl;
		m_choiceNextNode = &action;
	}
	
	void handle(ActionChoiceModifyRelation& action) {
		std::cout << "NEXT NODE CHOICE RELATION SET" << std::endl;
		m_choiceModifyRelation = &action;
	}

};

#endif // VN_STATE_CHOICES_H
