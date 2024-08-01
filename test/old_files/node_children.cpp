
#include "node_children.h"
#include "node.h"
#include "node_builder.h"

#include <algorithm>
#include <vector>
#include <memory>

NodeChildren::NodeChildren() :
	m_childrenViewer{},
	m_ownedChildren{},
	m_referencedChildren{}
{
};

int NodeChildren::size() {
	return m_childrenViewer.size();
}

void NodeChildren::updateChildrenViewer() {
	m_childrenViewer.clear();

	for (std::unique_ptr<Node>& nodeUniquePtr : m_ownedChildren) {
		m_childrenViewer.push_back(nodeUniquePtr.get());
	}

	for (Node* node : m_referencedChildren) {
		m_childrenViewer.push_back(node);
	}
}

void NodeChildren::addOwnedChild(Node* node) {
	m_ownedChildren.push_back(std::unique_ptr<Node>{node});
}

void NodeChildren::addReferencedChild(Node* node) {
	m_referencedChildren.push_back(node);
}

void NodeChildren::addChild(Node* parent, Node* child) {
	if (parent == nullptr || child == nullptr) { return; }

	if (NodeBuilder{ child }.isOwned()) {
		addReferencedChild(child);
	}
	else {
		addOwnedChild(child);
	}

	NodeBuilder{ child }.addParent(parent);

	updateChildrenViewer();
}

bool NodeChildren::removeOwnedChild(Node* child) {
	if (child == nullptr) { return false; }

	std::list<std::unique_ptr<Node>>::iterator listChild{
		std::find_if(m_ownedChildren.begin(), m_ownedChildren.end(), [&] (std::unique_ptr<Node>& node) { return node.get() == child; })
	};
	
	if (listChild != m_ownedChildren.end()) {
		// Remove the unique ptr without deleting the object
		(*listChild).release();

		m_ownedChildren.erase(listChild);
		return true;
	}

	return false;
}

bool NodeChildren::removeReferencedChild(Node* child) {
	if (child == nullptr) { return false; }

	std::list<Node*>::iterator listChild{ std::find(m_referencedChildren.begin(), m_referencedChildren.end(), child) };

	if (listChild != m_referencedChildren.end()) {
		m_referencedChildren.erase(listChild);
		return true;
	}

	return false;
}

void NodeChildren::removeChild(Node* parent, Node* child) {
	if (parent == nullptr || child == nullptr) { return; }

	bool isOwned{ removeOwnedChild(child) };
	bool isReferenced{ removeReferencedChild(child) };

	if (isOwned || isReferenced) {
		NodeBuilder{ child }.removeParent(parent);

		updateChildrenViewer();
	}
}

void NodeChildren::makeReferencedChildOwned(Node* child) {
	if (child == nullptr) { return; }

	// if the child is referenced but has no owner, then become the new owner
	if (std::find(m_referencedChildren.begin(), m_referencedChildren.end(), child) != m_referencedChildren.end()) {
		m_referencedChildren.remove(child);
		addOwnedChild(child);
	}
}

Node* NodeChildren::getChildById(int childId) {
	Node* child{ nullptr };

	for (Node* childIter : m_childrenViewer) {
		if (childIter == nullptr) {
			continue;
		}

		if (childIter->getId() == childId) {
			child = childIter;
			break;
		}
	}

	return child;
}

Node* NodeChildren::getChildByIndex(int index) {
	Node* child{ nullptr };
	
	int lastChildIndex = size() - 1;

	if (index >= 0 && index <= lastChildIndex) {
		child = m_childrenViewer[index];
	}

	return child;
}

Node* NodeChildren::operator[](int index) {
	return getChildByIndex(index);
}

std::vector<Node*>& NodeChildren::getChildrenView() {
	return m_childrenViewer;
}

void NodeChildren::print(int indentLevel) {
	for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
	std::cout << "Owned Children: ";
	for (std::unique_ptr<Node>& node : m_ownedChildren) {
		std::cout << node.get()->getId() << ' ';
	}
	std::cout << std::endl;

	for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
	std::cout << "Referenced Children: ";
	for (Node* node : m_referencedChildren) {
		std::cout << node->getId() << ' ';
	}
	std::cout << std::endl;
}