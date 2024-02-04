
#include "node_children.h"
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

int Node::getChildrenAmount() {
	return m_children.size(); 
}

void Node::print(bool printChildren) {
	std::cout << "node" << std::endl;
	std::cout << "	id:		 " << m_id << std::endl;
	std::cout << "	temp data: " << m_temp << std::endl;

	if (printChildren) {
		std::cout << "START children of " << m_temp << std::endl;
		for (int i{ 0 }; i < m_children.size(); i++) {
			m_children[i]->print(true);
		}
		std::cout << "END children of " << m_temp << std::endl;
	}
}