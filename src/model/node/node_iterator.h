#ifndef VN_NODE_ITERATOR_H
#define VN_NODE_ITERATOR_H

#include "node.h"
#include "state_subject.h"

class NodeIterator {
private:
	Node* m_node{ nullptr };
	int m_currentStep{ 0 };
	StateSubject* m_state{ nullptr };

public:
	NodeIterator(Node* node) : m_node{ node }
	{

	}

	void attatch(StateSubject* stateSubject) {
		m_state = stateSubject;
	}
	
	NodeState step() {
		NodeState actionResult = m_node->action(m_state, m_currentStep);
		m_currentStep++;

		return actionResult;
	}
};

#endif // VN_NODE_ITERATOR_H