
#ifndef BASE_NODE_H
#define BASE_NODE_H

#include "id.h"

#include <vector>
#include <string>
#include <iostream>
#include <memory>

/*
 * A story node, should have side effects on characters?
 * Can contain a normal text, characters, positions, player talking, etc..
 * Can also contain an opengl game? or maybe that should be a separate note type
 * Maybe node subtypes
 */
class Node {
private:
	int m_id{};
	Node* m_parent{};
	std::vector<std::unique_ptr<Node>> m_children{};

	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)

public:

	Node() : m_children{}, m_id{ IdGenerator::getId() } {

	}

	int getId() {
		return m_id;
	}

	// Engine Operations:

	// Usecase: A node is connected to a different parent
	void changeParent(Node* newParent) {
		m_parent = newParent;
	}
	
	void removeChild(int childId) {

	}

	void addChild(Node* child) {
		m_children.push_back(std::make_unique<Node>(child));
	}

	// function for writing out how to construct this node in cpp for to create runtime




	// Game Operations:
	Node* getParent() {
		return m_parent;
	}

	Node* getChild(int childIndex) {
		int lastVectorIndex = m_children.size() - 1;
		
		if (childIndex > lastVectorIndex || childIndex < 0) {
			return nullptr;
		}
		
		return m_children[childIndex].get();
	}

	int getChildrenAmount() {
		return m_children.size();
	}

	// Return children to display data from them for user to pick based on that
	std::vector<std::unique_ptr<Node>>& getChildren() {
		return m_children;
	}

	void action() {
		// pre
		// ret = theaction();
		// do post actions based on ret
	}


	virtual void displayNode() {
		std::cout << "base node" << std::endl;
	}

	friend std::ostream& operator<<(std::ostream& out, Node& node);
};







class ChapterNode: Node {
	virtual void displayNode() {
		std::cout << "chapter node" << std::endl;
	}
};

class OpenGLNode : Node {
	virtual void displayNode() {
		std::cout << "opengl node" << std::endl;
	}
};

#endif // BASE_NODE_H