#ifndef VN_NODE_ITERATOR_H
#define VN_NODE_ITERATOR_H

#include "node.h"

class StateSubject;

class NodeIterator {
private:
	Node* m_node{ nullptr };
	int m_currentStep{ 0 };
	StateSubject* m_stateSubject{ nullptr };

public:
	NodeIterator(Node* node) : m_node{ node }
	{

	}

	void attatchStateSubject(StateSubject* stateSubject) {
		m_stateSubject = stateSubject;
	}
	
	NodeState step() {
		NodeState actionResult = m_node->action(m_stateSubject, m_currentStep);
		m_currentStep++;

		return actionResult;
	}
};

#endif // VN_NODE_ITERATOR_H