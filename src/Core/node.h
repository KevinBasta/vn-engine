
#ifndef BASE_NODE_H
#define BASE_NODE_H

#include "id.h"
#include "node_children.h"

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
protected:
	int m_id{};
	bool m_isOwned{};
	Node* m_parent{};
	NodeChildren m_children{};

	std::string m_temp{};

	// pre actions (transitions, animations, etc..)
	// body actions (text, animations, etc..)
	// post actions (choises, bond mutations, transitions, animations, etc...)

public:
	Node();
	Node(std::string tempData);

public:
// Engine Operations:
	// Usecase: Connect a node to a different parent
	bool isOwned() { return m_isOwned; }
	bool setOwned(bool owned) { m_isOwned = owned; };

	void setParent(Node* parent) { m_parent = parent; }

	void addChild(Node* child);
	void removeChild(int childId);


public:
// Game Operations:
	int getId() { return m_id; }
	Node* getParent() { return m_parent; }
	Node* getChildByIndex(int childIndex);
	int getChildrenAmount() { return m_children.size(); }

	// Return children to display data from them for user to pick based on that
	// std::vector<std::unique_ptr<Node>>& getChildren() { return m_children; }

	void action() {
		// pre
		// ret = theaction();
		// do post actions based on ret
	}

	virtual void print(bool printChildren) {
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

	// maybe repurpose for writing out to file how to construct this node in cpp for to create runtime or make separte function
	friend std::ostream& operator<<(std::ostream& out, Node& node);
};







class ChapterNode: Node {
	virtual void print(bool printChildren) {
		std::cout << "chapter node" << std::endl;
		std::cout << "id:		 " << m_id << std::endl;
		std::cout << "temp data: " << m_temp << std::endl;
	}
};

class OpenGLNode : Node {
	virtual void print(bool printChildren) {
		std::cout << "opengl node" << std::endl;
		std::cout << "id:		 " << m_id << std::endl;
		std::cout << "temp data: " << m_temp << std::endl;
	}
};

#endif // BASE_NODE_H