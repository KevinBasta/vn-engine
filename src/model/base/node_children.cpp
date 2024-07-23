
#include "node_children.h"
#include "node.h"

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
	if (child->isOwned()) {
		addReferencedChild(child);
	}
	else {
		addOwnedChild(child);
	}

	child->addParent(parent);

	updateChildrenViewer();
}

bool NodeChildren::removeOwnedChild(Node* child) {
	if (child == nullptr) { return false; }

	Node* listChild{ nullptr };

	for (std::unique_ptr<Node>& nodeUniquePtr : m_ownedChildren) {
		if (nodeUniquePtr.get() == child) {
			listChild = nodeUniquePtr.release();

			// Not sure if this works
			m_ownedChildren.remove(nodeUniquePtr);
			break;
		}
	}

	return (listChild != nullptr);
}

bool NodeChildren::removeReferencedChild(Node* child) {
	if (child == nullptr) { return false; }

	Node* listChild{ nullptr };

	for (Node* node : m_referencedChildren) {
		if (node == child) {
			listChild = node;

			m_referencedChildren.remove(node);
			break;
		}
	}

	return (listChild != nullptr);
}

void NodeChildren::removeChild(Node* parent, Node* child) {
	if (parent == nullptr || child == nullptr) { return; }

	bool isOwned{ removeOwnedChild(child) };
	bool isReferenced{ removeReferencedChild(child) };

	if (isOwned || isReferenced) {
		child->removeParent(parent);

		updateChildrenViewer();
	}
}

void NodeChildren::refreshChildren() {
	std::vector<Node*> ownerlessChildren{};
	
	// if a child is referenced but has no owner, then become the new owner
	for (Node* node : m_referencedChildren) {
		if (node->isOwned() == false) {
			ownerlessChildren.push_back(node);
		}
	}

	for (Node* node : ownerlessChildren) {
		m_referencedChildren.remove(node);
		addOwnedChild(node);
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