
#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

#include <memory>

/*
 * A structure that allows forward and backwards progress in the narrative.
 * Forward progress to allow for progression based on choises.
 * Backwards progess to allow for looping back to a node if multiple child nodes need to 
 * be explored before forward progress continues.
 */

class Graph {
private:
	std::unique_ptr<node> m_head{};

public:
	Graph() : m_head{ new node } {

	}
};

#endif // GRAPH_H
