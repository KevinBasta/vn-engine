
#include "graph.h"
#include "graph_traverser.h"

#include <iostream>

bool GraphTraverser::pointToGraphHead() {
	if (!m_curGraph) {
		return false;
	}

	m_curNode = m_curGraph->getHead();

	if (!m_curNode) {
		return false;
	}

	m_nodeIterator = m_curNode->iter();

	return true;
}

bool GraphTraverser::pointToChild(int childIndex) {
	// handle no children case
	if (!m_curNode) {
		return false;
	}

	Node* child = m_curNode->getChildByIndex(childIndex);

	if (child) {
		m_curNode = child;
		m_nodeIterator = m_curNode->iter();

		return true;
	}

	return false;
}

bool GraphTraverser::pointToParent(int parentID) {
	if (m_curNode != m_curGraph->getHead()) {
		// needs to pick by parent id
		//m_current = m_current->getParent();
	}
	else {
		// Can allow to fetch from previous chapter if possible and needed
		std::cout << "Can't get parent of head node" << std::endl;
	}

	return false;
}
