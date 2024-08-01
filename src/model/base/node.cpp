
#include "node.h"
#include "node_runner.h"

#include <iostream>
#include <string>

NodeRunner Node::iter() { return NodeRunner(this); }

Node::Node() : 
	m_children{},
	m_parents{},
	m_id{ IdGenerator<Node>::getId() }
{

}

Node::Node(std::string tempData) : 
	m_children{}, 
	m_parents{},
	m_id{ IdGenerator<Node>::getId() },
	m_temp{ tempData }
{

}

id Node::getFirstChildId() {
	if (m_children.size() == 0) {
		// TODO: throw propper class
		throw "bad";
	}

	return *(m_children.begin());
}


Node::~Node() {
	std::cout << "Destructing NODE: " << m_temp << std::endl;
}

std::ostream& operator<<(std::ostream& out, Node& node) {
	std::cout << "a node" << std::endl;

	return out;
}

void Node::print(bool printChildren, int indentLevel) {
	std::cout << "called node print" << std::endl;
	/*
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
	*/
}