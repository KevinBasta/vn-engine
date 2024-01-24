
#ifndef BASE_NODE_H
#define BASE_NODE_H


#include <vector>
#include <string>
#include <iostream>

/*
 * A story node, should have side effects on characters?
 * Can contain a normal text, characters, positions, player talking, etc..
 * Can also contain an opengl game? or maybe that should be a separate note type
 * Maybe node subtypes
 */
class Node {
private:
	std::vector<Node&> m_children{};
	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)

public:

	Node() : m_children{} {

	}

	void addChild(Node& child) {
		m_children.push_back(child);
	}

	Node& selectNode(int n) {
		if (m_children.size() == 1) {
			return m_children[0];
		}



	}
	
	
	virtual void displayNode() {
		std::cout << "base node" << std::endl;
	}

	void action() {
		// pre
		// ret = theaction();
		// do post actions based on ret
	}
};

class ChapterNode: Node {
	virtual void displayNode() {
		std::count << "chapter node" << std::endl;
	}
};

class OpenGLNode : Node {
	virtual void displayNode() {
		std::count << "opengl node" << std::endl;
	}
};

#endif // BASE_NODE_H