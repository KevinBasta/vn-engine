
#include "state_relations.h"

#include "id.h"
#include "state_subject.h"
#include "model_subject.h"

#include <unordered_map>
#include <memory>

void StateRelations::handle(ActionRelationSetNextNode& action) {
	if (m_stateSubject == nullptr) { return; }

	for (auto condition : action.m_conditions) {
		bool result = eval(condition);

		if (result) {
			m_stateSubject->m_choices.setNextNodeId(action.m_nodeId);
			break;
		}
	}
}