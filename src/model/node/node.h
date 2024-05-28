
#ifndef BASE_NODE_H
#define BASE_NODE_H

#include "id.h"
#include "node_children.h"
#include "character.h"

#include "state_subject.h"
#include "model_common.h"

#include <vector>
#include <string>
#include <iostream>
#include <memory>


enum class NodeState {
	NODE_NOT_STARTED,
	NODE_STEP,
	NODE_CHOICE,
	NODE_END
};


class NodeChildren;
class NodeIterator;
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

public:
	Node();
	virtual ~Node();
	Node(std::string tempData);
	NodeIterator iterator();

public:
// Engine Operations:
	// Usecase: Connect a node to a different parent
	bool isOwned() { return m_isOwned; }
	void setOwned(bool owned) { m_isOwned = owned; }

	void setParent(Node* parent) { m_parent = parent; }

	void addChild(Node* child);
	void removeChild(int childId);


public:
// Game Operations:
	int getId() { return m_id; }
	Node* getParent() { return m_parent; } // TODO: can have multiple parents? hence must change
	Node* getChildByIndex(int childIndex);
	int getChildrenAmount();

	// Return children to display data from them for user to pick based on that
	// std::vector<std::unique_ptr<Node>>& getChildren() { return m_children; }

	virtual NodeState action(StateSubject* stateSubject, int stepIndex) = 0;
	//{
		// pre
		// ret = theaction();
		// do post actions based on ret
	//}

	virtual void print(bool printChildren, int indentLevel=0);

	// maybe repurpose for writing out to file how to construct this node in cpp for to create runtime or make separte function
	friend std::ostream& operator<<(std::ostream& out, Node& node);
};

#endif // BASE_NODE_H