#ifndef VN_GRAPH_ITERATOR_H
#define VN_GRAPH_ITERATOR_H

#include "graph.h"
#include "node.h"
#include "node_iterator.h"
#include "state_subject.h"

class GraphIterator {
private:
	Graph* m_graph{ nullptr };
	// node iterator?
	Node* m_current{ nullptr };

	StateSubject* m_state{ nullptr };

	void pointToHead() {
		// null checks needed
		
		m_current = m_graph->getHead();
	}

public: 
	GraphIterator(Graph* graph) : m_graph{ graph } {
		pointToHead();
	}

	void attatch(StateSubject* stateSubject) {
		m_state = stateSubject;
	}

	void next(int childIndex = 0) {
				
	}
};


#endif // VN_GRAPH_ITERATOR_H
