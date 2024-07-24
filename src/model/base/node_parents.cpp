#include "node.h"
#include "node_parents.h"
#include "node_stray.h"
#include "node_builder.h"

#include <iostream>

void NodeParents::addParent(Node* parent, Node* currentNode) {
	if (parent == nullptr) { return; }

	if (m_parents.size() == 0) {
		NodeStray::removeStray(currentNode);
	}
	
	if (m_owner == nullptr) {
		m_owner = parent;
	}

	// TODO: only if m_parents doesn't have this parent
	m_parents.push_back(parent);
}

void NodeParents::removeParent(Node* parent, Node* currentNode) {
	if (parent == nullptr || currentNode == nullptr) { return; }

	m_parents.remove(parent);

	if (m_owner == parent) {
		if (m_parents.size() == 0) {
			m_owner = nullptr;
			
			// Keep a live pointer to the now orphaned child
			NodeStray::addStray(currentNode);
			return;
		}
		else {
			// Set the 0th parent as the new owner
			m_owner = m_parents.front();

			NodeBuilder{ m_owner }.makeReferencedChildOwned(currentNode);
		}
	}
}
