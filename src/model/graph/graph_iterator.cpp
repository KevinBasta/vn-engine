
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

/*
 * Public Interface 
 */


void GraphIterator::first() {
	// m_graph null check needed
	m_current = m_graph->getHead();
	m_nodeIterator = m_current->iterator();
	m_nodeIterator.attatch(m_state);
}

void GraphIterator::jumpToNode(int nodeId) {
	Node* node{ m_graph->getNodeById(nodeId) };
	
	if (node) {
		m_current = node;
		m_nodeIterator = m_current->iterator();
		m_nodeIterator.attatch(m_state);
	}
	else {
		std::cout << "Node not found" << std::endl;
		first();
	}
}

void GraphIterator::step() {
	// can return a graph iterator status saying that no more children so parent can go to next chapter
	NodeState stepResult{ m_nodeIterator.step() };

	if (stepResult == NodeState::NODE_END) {
		pointToChild(0);
	}
}
