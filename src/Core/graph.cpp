
#include "graph.h"
#include "node.h"

#include <memory>
#include <unordered_set>
#include <iostream>

Graph::Graph() :
	m_head{},
	m_current{ nullptr }
{
}


//bool Graph::insertChildNode(Node* node) {
//	m_current
//}

void Graph::setHeadNode(Node* node) {
	// Does this deallocate the previous uniqueptr object if there was one?
	m_head = std::make_unique<Node>(node);
}

// Game Operations:

// Usecase: Forward progress in the story
bool Graph::pointToChild(int nodeIndex) {
	// handle no children case
	m_current = m_current->getChildByIndex(nodeIndex);
}

// Usecase: backward progress in the story
void Graph::pointToParent() {
	if (m_current != m_head.get()) {
		m_current = m_current->getParent();
	}
	else {
		// Can allow to fetch from previous chapter if possible and needed
		std::cout << "Can't get parent of head node" << std::endl;
	}
}


// Set m_current to the node with id targetNodeId if it exists
// can be switched to bfs later, although it's a tradeoff, if can be made tail recursive would be better
void Graph::dfs(int targetNodeId, Node* nodeChecking, std::unordered_set<int>& visitedNodes) {
	// there might be a bug in this function where unordered_set is placed elsewhere? may not apply to unordered_set
	if (m_current != nullptr) {
		return;
	}

	if (nodeChecking->getId() == targetNodeId) {
		// Check the node
		m_current = nodeChecking;
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

// Usecase: Viewing old nodes or loading from save file
bool Graph::pointToNode(int nodeId) {
	using visitedNodeId = int;

	// Required to keep track of visited nodes since graph can have cycles
	std::unordered_set<visitedNodeId> visitedNodes{};

	m_current = nullptr;

	dfs(nodeId, m_head.get(), visitedNodes);

	if (m_current == nullptr) {
		std::cout << "Node not found" << std::endl;
		pointToHead();
		return false;
	}
	else {
		std::cout << "Node found" << std::endl;
		return true;
	}
}



std::ostream& operator<<(std::ostream& out, Graph& graph) {
	std::cout << "Graph:" << std::endl;
	std::cout << "\tHead:" << *(graph.m_head.get()) << std::endl;
	
	std::cout << "\tCurrent:" << std::endl;
	std::cout << *(graph.m_current)->print(false);
}