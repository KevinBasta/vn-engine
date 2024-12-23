
#ifndef BASE_NODE_H
#define BASE_NODE_H

#include "index.h"

#include "linkable.h"

#include <string>
#include <iostream>

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
class Node : public Linkable {
private:
	friend class NodeBuilder;

public:	
	// Temp
	std::string m_temp{};

public:
	Node() {};
	Node(std::string tempData) : m_temp{ tempData } {};
	virtual ~Node();
	NodeRunner iter();

public:
// Game Operations:
	virtual NodeState action(StateSubject* stateSubject, int stepIndex) = 0;
	virtual index getTotalSteps() = 0;

	// maybe repurpose for writing out to file how to construct this node in cpp for to create runtime or make separte function
	friend std::ostream& operator<<(std::ostream& out, Node& node);
};

#endif // BASE_NODE_H