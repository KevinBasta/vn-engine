
#include "node_children.h"
#include "node.h"
#include "node_runner.h"

#include <iostream>
#include <string>

NodeRunner Node::iter() { return NodeRunner(this); }

std::ostream& operator<<(std::ostream& out, Node& node) {
	std::cout << "a node" << std::endl;

	return out;
}

Node::Node() : 
	m_children{},
	m_id{ IdGenerator<Node>::getId() }
{

}

Node::Node(std::string tempData) : 
	m_children{}, 
	m_id{ IdGenerator<Node>::getId() },
	m_temp{ tempData }
{

}

Node::~Node() {
	std::cout << "Destructing NODE: " << m_temp << std::endl;
}


Node* Node::getChildByIndex(int childIndex) {
	return m_children[childIndex];
}

Node* Node::getChildById(int childId) {
	return m_children.getChildById(childId);
}

void Node::addChild(Node* child) {
	m_children.addChild(this, child);
}

int Node::getChildrenAmount() {
	return m_children.size(); 
}

void Node::print(bool printChildren, int indentLevel) {
	for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
	std::cout << "node id: " << m_id << std::endl;
	
	m_children.print(indentLevel);
	
	for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
	std::cout << "temp data: " << m_temp << std::endl;


	if (printChildren) {
		for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
		std::cout << "{" << std::endl;
		
		// Only print the children of the owned children. Otherwise will have
		// duplicate prints and possible infinite loops. Will need to temporarily 
		// relax the node children interface
		std::vector<std::unique_ptr<Node>>& ownedChildren = m_children.getOwnedChildren();
		for (size_t i = 0; i < ownedChildren.size(); i++) {
			ownedChildren[i].get()->print(printChildren, indentLevel + 1);
		}

		std::vector<Node*>& referencedChildren = m_children.getReferencedChildren();
		for (size_t i = 0; i < referencedChildren.size(); i++) {
			referencedChildren[i]->print(false, indentLevel + 1);
		}

		for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
		std::cout << "}" << std::endl;
	}
	else {
		for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
		std::cout << "{children not printed...}" << std::endl;
	}
}