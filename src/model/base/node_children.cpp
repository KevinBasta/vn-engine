
#include "node_children.h"
#include "node.h"

#include <vector>
#include <memory>

NodeChildren::NodeChildren(): 
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
		// May eliminate the m_isOwned property since can check if parent is set
		child->setOwned(true);
		child->setParent(parent);
		
		addOwnedChild(child);
	}

	updateChildrenViewer();
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