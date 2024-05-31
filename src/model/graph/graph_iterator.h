#ifndef VN_GRAPH_ITERATOR_H
#define VN_GRAPH_ITERATOR_H

#include "graph.h"
#include "node.h"
#include "node_iterator.h"

class StateSubject;

enum class GraphState {
	GRAPH_STEP,
	GRAPH_END
};

class GraphIterator {
private:
	Graph* m_graph{ nullptr };
	Node* m_current{ nullptr };
	NodeIterator m_nodeIterator{ nullptr };

	StateSubject* m_stateSubject{ nullptr };

	bool pointToParent(int parentID);
	bool pointToChild(int childIndex = 0);

public: 
	GraphIterator(Graph* graph) : m_graph{ graph } {
		if (graph) {
			first();
		}
	}


	/*
	 * Public Interface
	 */

	void attatchStateSubject(StateSubject* stateSubject) {
		m_stateSubject = stateSubject;
	}

	void first() {
		// m_graph null check needed
		m_current = m_graph->getHead();
		m_nodeIterator = m_current->iterator();
		m_nodeIterator.attatchStateSubject(m_stateSubject);
	}

	GraphState step() {
		// can return a graph iterator status saying that no more children so parent can go to next chapter
		NodeState stepResult{ m_nodeIterator.step() };

		if (stepResult == NodeState::NODE_END) {
			pointToChild(0);
		}

		return GraphState::GRAPH_STEP;
	}


	void jumpToNode(int nodeId) {
		Node* node{ m_graph->getNodeById(nodeId) };

		if (node) {
			m_current = node;
			m_nodeIterator = m_current->iterator();
			m_nodeIterator.attatchStateSubject(m_stateSubject);
		}
		else {
			std::cout << "Node not found" << std::endl;
			first();
		}
	}
};


#endif // VN_GRAPH_ITERATOR_H
