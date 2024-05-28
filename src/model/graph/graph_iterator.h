#ifndef VN_GRAPH_ITERATOR_H
#define VN_GRAPH_ITERATOR_H

#include "graph.h"
#include "node.h"
#include "node_iterator.h"
#include "state_subject.h"

class GraphIterator {
private:
	Graph* m_graph{ nullptr };
	Node* m_current{ nullptr };
	NodeIterator m_nodeIterator{ nullptr };

	StateSubject* m_state{ nullptr };

	bool pointToParent(int parentID);
	bool pointToChild(int childIndex = 0);

public: 
	GraphIterator(Graph* graph) : m_graph{ graph } {
	}

	void attatchState(StateSubject* stateSubject) {
		m_state = stateSubject;
	}

	// interface
	void first();
	void step();
	void jumpToNode(int nodeId);
};


#endif // VN_GRAPH_ITERATOR_H
