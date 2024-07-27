#ifndef VN_STATE_CHOICES_H
#define VN_STATE_CHOICES_H

#include "chapter_node_types.h"

#include <vector>

class StateSubject;

class StateChoices {
public:
	StateSubject* m_stateSubject{ nullptr };
	StateChoices(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}

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

	void chooseUpChoice();
	void chooseDownChoice();

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
};

#endif // VN_STATE_CHOICES_H
