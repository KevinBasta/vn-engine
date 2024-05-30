
#include "graph.h"
#include "graph_iterator.h"

#include <iostream>

bool GraphIterator::pointToParent(int parentID) {
	if (m_current != m_graph->getHead()) {
		// needs to pick by parent id
		//m_current = m_current->getParent();
	}
	else {
		// Can allow to fetch from previous chapter if possible and needed
		std::cout << "Can't get parent of head node" << std::endl;
	}

	return false;
}

bool GraphIterator::pointToChild(int childIndex) {
	// handle no children case
	Node* child = m_current->getChildByIndex(childIndex);

	if (child) {
		m_current = child;
		return true;
	}

	return false;
}



