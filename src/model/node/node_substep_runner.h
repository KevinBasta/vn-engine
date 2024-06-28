#ifndef VN_NODE_SUBSTEP_ITERATOR_H
#define VN_NODE_SUBSTEP_ITERATOR_H

#include "node.h"

// Circular dependancy resolutions:
class StateSubject;

//class NodeSubStepRunner {
//private:
//	Node* m_node{ nullptr };
//	int m_currentStep{ 0 }; // setting this to const causes the = operator to be deleted
//	int m_currentSubStep{ 0 };
//	NodeState m_latestAction{ NodeState::NODE_NOT_STARTED };
//
//public:
//	NodeSubStepRunner(Node* node, int currentStep) : m_node{ node }, m_currentStep{ currentStep }
//	{
//	}
//
//	NodeState subStep(StateSubject* stateSubject);
//};

#endif // VN_NODE_SUBSTEP_ITERATOR_H