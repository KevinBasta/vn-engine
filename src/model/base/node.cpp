
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

void Node::removeChild(Node* child) {
	m_children.removeChild(this, child);
};

void Node::makeReferencedChildOwned(Node* node) {
	m_children.makeReferencedChildOwned(node);
};


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
		std::list<std::unique_ptr<Node>>& ownedChildren = m_children.getOwnedChildren();
		for (auto& child : ownedChildren) {
			child.get()->print(printChildren, indentLevel + 1);
		}
		
		std::list<Node*>& referencedChildren = m_children.getReferencedChildren();
		for (auto& child : referencedChildren) {
			child->print(false, indentLevel + 1);
		}
		
		for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
		std::cout << "}" << std::endl;
	}
	else {
		for (int i = 0; i < indentLevel; i++) { std::cout << '\t'; };
		std::cout << "{children not printed...}" << std::endl;
	}
}