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
	ActionChooseNode* m_nodeChoices{ nullptr };

	int m_currentChoiceIndex{ 0 };
	id m_currentChoiceId{ 0 };

public:
	StateChoices(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}
	
public:
	//
	// Keep record of what chioces were made for save file
	//
	std::vector<int> m_chapterChoicesRecord{};

	void appendChapterChoice() {
		m_chapterChoicesRecord.push_back(getChoiceId());
	}

public:
	

	bool isChoiceActive() {
		return m_activeChoice;
	}

	int getChoiceIndex() {
		return m_currentChoiceIndex;
	}

	ActionChooseNode* getNodeChoices() {
		return m_nodeChoices;
	}

	void recordAndDisableChoice() {
		appendChapterChoice();
		m_activeChoice = false;
		m_nodeChoices = nullptr;
	}

public:
	//
	// Controller interface
	//
	void chooseUpChoice();
	void chooseDownChoice();
	void updateChoiceId() {
		if (m_activeChoice == false || m_nodeChoices == nullptr) {
			m_currentChoiceId = 0;
			return;
		}

		if (m_currentChoiceIndex < 0 || m_currentChoiceIndex >= m_nodeChoices->m_choices.size()) {
			m_currentChoiceId = 0;
			return;
		}

		m_currentChoiceId = m_nodeChoices->m_choices[m_currentChoiceIndex].m_nodeID;
	}

	id getChoiceId() {
		return m_currentChoiceId;
	}

	void handle(ActionChooseNode& action) {
		m_activeChoice = true;

		m_nodeChoices = &action;

		m_currentChoiceIndex = 0;
		updateChoiceId();
	}
};

#endif // VN_STATE_CHOICES_H
