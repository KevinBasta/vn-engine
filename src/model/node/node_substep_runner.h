#ifndef VN_NODE_SUBSTEP_ITERATOR_H
#define VN_NODE_SUBSTEP_ITERATOR_H

#include "node.h"

// Circular dependancy resolutions:
class StateSubject;

class NodeSubStepRunner {
private:
	Node* m_node{ nullptr };
	int m_currentStep{ 0 }; // setting this to const causes the = operator to be deleted
	int m_currentSubStep{ 1 };
	NodeState m_latestAction{ NodeState::NODE_NOT_STARTED };

public:
	NodeSubStepRunner(Node* node, int currentStep) : m_node{ node }, m_currentStep{ currentStep }
	{
	}

	NodeState subStep(StateSubject* stateSubject) {
		if (m_latestAction == NodeState::NODE_SUBSTEP_END) {
			return m_latestAction;
		}

		if (m_node == nullptr || stateSubject == nullptr) {
			return m_latestAction;
		}

		m_latestAction = m_node->subAction(stateSubject, m_currentStep, m_currentSubStep);
		m_currentSubStep++;

		return m_latestAction;
	}
};

#endif // VN_NODE_SUBSTEP_ITERATOR_H