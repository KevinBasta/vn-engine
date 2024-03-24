
#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

#include <memory>
#include <iostream>
#include <unordered_set>

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
	std::unique_ptr<Node> m_head{};
	Node* m_current{};

public:
	Graph();

public:
// Engine Operations:
	// Usecase: modify or populate the m_current
	Node* getCurrentNode() { return m_current; }

	// Usecase: insert child node into m_current
	// bool insertChildNode(Node* node); // Does this make sense? Can't this be done in the node object itself?

	void setHeadNode(Node* node);

public:
// Game Operations:
	// Usecase: after constructing the tree, prepare it for traversal
	void pointToHead() { m_current = m_head.get(); };
	
	// Usecase: forward progress in the story
	bool pointToChild(int nodeId);

	// Usecase: backward progress in the story
	void pointToParent();
	
	// Usecase: viewing old nodes or loading from save file
	bool pointToNode(int nodeId);

private:
	// Recursive function only for use with setCurrentNode
	void dfs(int targetNodeId, Node* nodeChecking, std::unordered_set<int>& visitedNodes);

public:
	// Operators:
	friend std::ostream& operator<<(std::ostream& out, Graph& graph);
};

#endif // GRAPH_H
