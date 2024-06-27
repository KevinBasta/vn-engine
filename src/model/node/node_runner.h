#ifndef VN_NODE_ITERATOR_H
#define VN_NODE_ITERATOR_H

#include "node.h"

// Circular dependancy resolutions:
class StateSubject;

/*
 * This class is for stepping through the node
 * actions and updating the state(s) attached
 */
class NodeRunner {
private:
	Node* m_node{ nullptr };
	int m_currentStep{ 0 };
	int m_currentSubStep{ 0 };
	NodeState m_latestAction{ NodeState::NODE_NOT_STARTED };

public:
	NodeRunner(Node* node) : m_node{ node } 
	{
	}
	
	NodeState step(StateSubject* stateSubject) {
		if (m_latestAction == NodeState::NODE_END) {
			return m_latestAction;
		}
		
		m_latestAction = m_node->action(stateSubject, m_currentStep);
		m_currentStep++;

		return m_latestAction;
	}

	NodeState subStep(StateSubject* stateSubject) {
		if (m_latestAction == NodeState::NODE_END) {
			return m_latestAction;
		}

		m_latestAction = m_node->subAction(stateSubject, m_currentStep, m_currentSubStep);
		m_currentStep++;
		m_currentSubStep++;

		return m_latestAction;
	}
};

#endif // VN_NODE_ITERATOR_H