
#include "graph.h"

#include "node.h"
#include "graph_iterator.h"

#include <memory>
#include <unordered_set>
#include <iostream>

GraphIterator Graph::iterator() { return GraphIterator(this);	}

std::ostream& operator<<(std::ostream& out, Graph& graph) {
	std::cout << "Graph:" << std::endl;
	std::cout << "Head:" << std::endl;
	(graph.m_head.get())->print(true);
	std::cout << std::endl;

	//std::cout << "Current:" << std::endl;
	//if (graph.m_current != nullptr) {
	//	(graph.m_current)->print(false);
	//	std::cout << std::endl;
	//}

	return out;
}

// Usecase: Viewing old nodes or loading from save file
Node* Graph::getNodeById(int nodeId) {
	using visitedNodeId = int;

	// Required to keep track of visited nodes since graph can have cycles
	std::unordered_set<visitedNodeId> visitedNodes{};

	m_targetSearchNode = nullptr;

	dfs(nodeId, m_head.get(), visitedNodes);

	if (m_targetSearchNode == nullptr) {
		std::cout << "Node NOT found" << std::endl;
	}
	else {
		std::cout << "Node found" << std::endl;
	}

	return m_targetSearchNode;
}

// Set m_current to the node with id targetNodeId if it exists
// can be switched to bfs later, although it's a tradeoff, if can be made tail recursive would be better
void Graph::dfs(int targetNodeId, Node* nodeChecking, std::unordered_set<int>& visitedNodes) {
	// there might be a bug in this function where unordered_set is placed elsewhere? may not apply to unordered_set
	if (m_targetSearchNode != nullptr) {
		return;
	}

	if (nodeChecking->getId() == targetNodeId) {
		// Check the node
		m_targetSearchNode = nodeChecking;
	}
	else {
		// Check the node's children
		int childrenAmount = nodeChecking->getChildrenAmount();

		for (int i{ 0 }; i < childrenAmount; i++) {
			Node* child = nodeChecking->getChildByIndex(i);
			
			if (visitedNodes.find(child->getId()) == visitedNodes.end()) {
				dfs(targetNodeId, child, visitedNodes);
			}
		}
	}

	return;
}
