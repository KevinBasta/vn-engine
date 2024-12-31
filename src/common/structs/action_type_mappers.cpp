
#include "action_type_mappers.h"
#include "action_type_mappers_helpers.h"
#include "engine_node_action_fields.h"

#include <string>

template <class T>
ActionHelper::ActionHelper(std::in_place_type_t<T>) {
	this->getType = []() { return ActionToType<T>::type; };
	this->getName = []() { return ActionToActionName<T>::name; };
	this->drawNew = []() { return ActionField<T>::drawField(); };

	this->drawExisting = [](ChapterNode* node, int index) { return ActionField<T>::drawField(node, index); };
	
	this->performMove = [](ActionDragDropPayload payload) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return; }

		ChapterNodeBuilder{ node }.moveStep<T>(payload);
	};

	this->equals =  [](ActionHelper helper) { return std::string(helper.getName()) == std::string(ActionToActionName<T>::name); };
}