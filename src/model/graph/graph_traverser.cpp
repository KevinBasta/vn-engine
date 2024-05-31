
#include "graph.h"
#include "graph_traverser.h"

#include <iostream>

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

bool GraphTraverser::pointToChild(int childIndex) {
	// handle no children case
	Node* child = m_curNode->getChildByIndex(childIndex);

	if (child) {
		m_curNode = child;
		return true;
	}

	return false;
}



