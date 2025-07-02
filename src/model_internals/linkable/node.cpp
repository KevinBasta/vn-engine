
#include "node.h"
#include "node_runner.h"

#include <iostream>
#include <string>

NodeRunner Node::iter() { return NodeRunner(this); }

Node::~Node() {
	std::cout << "Destructing NODE: " << m_name << std::endl;
}

std::ostream& operator<<(std::ostream& out, Node& node) {
	std::cout << "a node" << std::endl;

	return out;
}
