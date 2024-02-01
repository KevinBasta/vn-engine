
#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

#include <memory>
#include <iostream>

/*
 * A structure that allows forward and backwards progress in the narrative.
 * Forward progress to allow for progression based on choises.
 * Backwards progess to allow for looping back to a node if multiple child nodes need to 
 * be explored before forward progress continues.
 */

// Design choice: enforce that cycles must be able to reach an end?
// Can add flags somewhere in the case that a flag is set, cycle back to an old node, then do something slightly different based on flag

class Graph {
private:
	std::shared_ptr<Node> m_head{};
	std::shared_ptr<Node> m_current{};

public:
	Graph();

public:
	// Runtime Engine Operations:

	// Usecase: modify or populate the m_current
	Node* getCurrentNode() { return m_current.get(); }

	// Usecase: insert child node into m_current
	bool insertChildNode(std::shared_ptr<Node> node);

	// Usecase: after constructing the tree, prepare it for traversal
	bool setCurrentNodeToHead();

public:
	// Runtime Game Operations:
	
	// Usecase: forward progress in the story
	bool goToChildNode(int nodeId);

	// Usecase: backward progress in the story
	void goToParentNode();
	
	// Usecase: viewing old nodes or loading from save file
	bool setCurrentNode(int nodeId);

public:
	// Operators:
	friend std::ostream& operator<<(std::ostream& out, Graph& graph);
};

#endif // GRAPH_H
