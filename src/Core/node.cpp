
#include "node.h"

std::ostream& operator<<(std::ostream& out, Node& node) {
	std::cout << "a node" << std::endl;

	return out;
}

Node::Node() : m_children{}, m_id{ IdGenerator::getId() } {

}

Node* Node::getChild(int childIndex) {
	int lastVectorIndex = m_children.size() - 1;

	if (childIndex > lastVectorIndex || childIndex < 0) {
		return nullptr;
	}

	return m_children[childIndex].get();
}

void Node::addChild(Node* child) {
	m_children.push_back(std::make_unique<Node>(child));
}