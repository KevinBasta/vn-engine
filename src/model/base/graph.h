
#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

#include <memory>
#include <iostream>
#include <unordered_set>


class GraphTraverser;

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
	Node* m_targetSearchNode{ nullptr };

public:
// Game Operations:
	Node* getHead() { return m_head.get(); }
	GraphTraverser iter();

public:
// Engine Operations:
	void setHeadNode(Node* node) { m_head.reset(node); }
	friend std::ostream& operator<<(std::ostream& out, Graph& graph);

/* 
 * Algorithm for iterator 
 */
public:
	// Usecase: viewing old nodes or loading from save file
	Node* getNodeById(int nodeId);

private:
	// Recursive function only for use with getNodeById
	void dfs(int targetNodeId, Node* nodeChecking, std::unordered_set<int>& visitedNodes);
};

#endif // GRAPH_H
