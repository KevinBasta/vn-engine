#ifndef VN_GRAPH_ITERATOR_H
#define VN_GRAPH_ITERATOR_H

#include "graph.h"
#include "node.h"
#include "node_runner.h"

class StateSubject;

enum class GraphState {
	GRAPH_NOT_STARTED,
	GRAPH_STEP,
	GRAPH_END
};

class GraphTraverser {
private:
	Graph*		m_curGraph{ nullptr };
	GraphState  m_curGraphState{ GraphState::GRAPH_NOT_STARTED };

	Node*		m_curNode{ nullptr };
	NodeRunner	m_nodeIterator{ nullptr };

private:
	// flase for fail, true for success
	bool pointToGraphHead();
	bool pointToChild(int childIndex = 0);

	// TODO: implement point to parent
	bool pointToParent(int parentID);

public: 
	GraphTraverser(Graph* graph) : m_curGraph{ graph } {
		pointToGraphHead();
	}

	GraphState step(StateSubject* stateSubject) {
		if (m_curGraphState == GraphState::GRAPH_END) {
			return m_curGraphState;
		}
		
		NodeState stepResult = m_nodeIterator.step(stateSubject);

		if (stepResult == NodeState::NODE_END) {
			bool nextNodeInit = pointToChild();
			if (!nextNodeInit) {
				std::cout << "GRAPH DONE" << std::endl;
				return GraphState::GRAPH_END;
			}
			else {
				// If the node is done, jump to the next node
				return step(stateSubject);
			}
		}

		return GraphState::GRAPH_STEP;
	}

	GraphState subStep(StateSubject* stateSubject) {

	}

	/*void jumpToNode(int nodeId) {
		Node* node{ m_curGraph->getNodeById(nodeId) };

		if (node) {
			m_curNode = node;
			m_nodeIterator = m_curNode->iter();
		}
		else {
			std::cout << "Node not found" << std::endl;
			pointToGraphHead();
		}
	}*/
};


#endif // VN_GRAPH_ITERATOR_H
