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


	/*
	 * Public Interface
	 */

	void attatchState(StateSubject* stateSubject) {
		m_state = stateSubject;
	}

	void first() {
		// m_graph null check needed
		m_current = m_graph->getHead();
		m_nodeIterator = m_current->iterator();
		m_nodeIterator.attatch(m_state);
	}

	void step() {
		// can return a graph iterator status saying that no more children so parent can go to next chapter
		NodeState stepResult{ m_nodeIterator.step() };

		if (stepResult == NodeState::NODE_END) {
			pointToChild(0);
		}
	}


	void jumpToNode(int nodeId) {
		Node* node{ m_graph->getNodeById(nodeId) };

		if (node) {
			m_current = node;
			m_nodeIterator = m_current->iterator();
			m_nodeIterator.attatch(m_state);
		}
		else {
			std::cout << "Node not found" << std::endl;
			first();
		}
	}
};


#endif // VN_GRAPH_ITERATOR_H
