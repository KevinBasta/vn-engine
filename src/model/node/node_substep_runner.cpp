
#include "node.h"
#include "node_substep_runner.h"
#include "state_subject.h"

NodeState NodeSubStepRunner::subStep(StateSubject* stateSubject) {
	if (m_latestAction == NodeState::NODE_SUBSTEP_END) {
		return m_latestAction;
	}

	if (m_node == nullptr || stateSubject == nullptr) {
		return m_latestAction;
	}

	m_latestAction = m_node->subAction(stateSubject, m_currentStep, m_currentSubStep);
	m_currentSubStep++;

	if (m_latestAction == NodeState::NODE_SUBSTEP_END) {
		stateSubject->clearSubStep();
	}

	return m_latestAction;
}