
#include "node.h"

#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& out, Node& node) {
	std::cout << "a node" << std::endl;

	return out;
}

Node::Node() : 
	m_children{},
	m_id{ IdGenerator::getId() }
{

}

Node::Node(std::string tempData) : 
	m_children{}, 
	m_id{ IdGenerator::getId() },
	m_temp{ tempData }
{

}

Node* Node::getChildByIndex(int childIndex) {
	return m_children[childIndex];
}

void Node::addChild(Node* child) {
	m_children.addChild(this, child);
}