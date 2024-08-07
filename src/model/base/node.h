
#ifndef BASE_NODE_H
#define BASE_NODE_H

#include "id.h"
#include "character.h"

#include <set>
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
	std::set<id> m_parents{};
	std::set<id> m_children{};

	// Temp
	std::string m_temp{};

public:
	Node();
	Node(std::string tempData);
	virtual ~Node();
	NodeRunner iter();

public:
// Game Operations:
	int getId() { return m_id; }
	
	id getParentIdByIndex(int parentIndex);
	id getParentIdById(int parentId);

	int getChildrenAmount() { return m_children.size(); }
	id getFirstChildId();
	id getChildIdById(int childId);

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