
#include "action_type_mappers.h"
#include "action_type_mappers_helpers.h"
#include "engine_node_action_fields.h"

#include <string>
#include <functional>

template <class T>
ActionHelper::ActionHelper(std::in_place_type_t<T>) {
	this->getType = []() { return ActionToType<T>::type; };
	this->getName = []() { return ActionToActionName<T>::name; };

	this->drawNew = []() { return ActionField<T>::drawField(); };
	this->drawExisting = [](ChapterNode* node, int index) { return ActionField<T>::drawField(node, index); };
	
	this->addStaticObjToNodeAtStep = [](id nodeId, index stepIndex) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(nodeId));
		if (node == nullptr) { return; }

		T obj = ActionField<T>::getStaticObj();

		if (ActionToType<T>::type == ActionAmount::SINGLE) {
			ChapterNodeBuilder{ node }.replaceAction<T>(stepIndex, obj);
		}
		else if (ActionToType<T>::type == ActionAmount::VECTOR) {
			ChapterNodeBuilder{ node }.addAction<T>(stepIndex, obj);
		}
	};
	this->performMove = [](ActionDragDropPayload payload) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return ChapterNodeBuilder{ node }.moveAction<T>(payload);
	};
	this->performCopy = [](ActionDragDropPayload payload) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return ChapterNodeBuilder{ node }.copyAction<T>(payload);
	};
	this->performSwap = [](ActionDragDropPayload payload) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return ChapterNodeBuilder{ node }.swapAction<T>(payload);
	};
	this->forceSwap = [](ActionDragDropPayload payload) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return ChapterNodeBuilder{ node }.forceSwap<T>(payload);
	};
	this->performDelete = [](ActionDragDropPayload payload) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return ChapterNodeBuilder{ node }.deleteAction<T>(payload);
	};
	this->containsStep = [](id nodeId, index stepIndex) {
		ChapterNode* node = static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(nodeId));
		if (node == nullptr) { return false; }

		return ChapterNodeBuilder{ node }.containsStep<T>(stepIndex);
	};

	this->equals =  [](ActionHelper helper) { return std::string(helper.getName()) == std::string(ActionToActionName<T>::name); };
}