
#include "graph.h"
#include "node.h"

#include <memory>
#include <unordered_set>

Graph::Graph() : m_head{ std::make_shared<Node>() } {
	m_current = m_head;
}

// Usecase: Forward progress in the story
bool Graph::goToChildNode(int nodeId) {

}

// Usecase: backward progress in the story
void Graph::goToParentNode() {

}

// Jump to arbitrary node (walk the tree/graph) [keep track of nodes visited so not to visit them again in the graph using hash]

// Usecase: Viewing old nodes or loading from save file
bool Graph::setCurrentNode(int nodeId) {
	using visitedNodeId = int;
	
	// Required to keep track of visited nodes since graph can have cycles
	std::unordered_set<visitedNodeId> visitedNodes{};


}