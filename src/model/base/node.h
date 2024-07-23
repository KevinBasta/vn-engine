
#ifndef BASE_NODE_H
#define BASE_NODE_H

#include "id.h"
#include "node_children.h"
#include "node_parents.h"
#include "character.h"

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

class StateSubject;
class NodeChildren;
class NodeParents;
class NodeRunner;
class NodeBuilder;
/*
 * A story node, should have side effects on characters?
 * Can contain a normal text, characters, positions, player talking, etc..
 * Can also contain an opengl game? or maybe that should be a separate note type
 * Maybe node subtypes
 */
class Node {
private:
	friend class NodeBuilder;

protected:
	int m_id{};
	
	// For the purposes of traversal 
	NodeParents m_parents{};
	NodeChildren m_children{};

	// Temp
	std::string m_temp{};

public:
	Node();
	virtual ~Node();
	Node(std::string tempData);
	NodeRunner iter();

public:
// Engine Operations:
	// Usecase: Connect a node to a different parent
	bool isOwned() { return m_parents.hasOwner(); }
	void addParent(Node* parent) { m_parents.addParent(parent, this); }
	void removeParent(Node* parent) { m_parents.removeParent(parent, this); }

	void addChild(Node* child);
	void removeChild(Node* child);
	void refreshChildren();

public:
// Game Operations:
	int getId() { return m_id; }
	Node* getParentByIndex(int parentIndex);
	Node* getParentById(int parentId);
	Node* getChildByIndex(int childIndex);
	Node* getChildById(int childId);
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