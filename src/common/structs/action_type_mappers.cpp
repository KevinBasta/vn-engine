
#include "action_type_mappers.h"
#include "action_type_mappers_helpers.h"
#include "engine_scene_action_editor.h"

#include <string>
#include <functional>

template <class T>
ActionHelper::ActionHelper(std::in_place_type_t<T>) {
	this->getType = []() { return ActionToType<T>::type; };
	this->getName = []() { return ActionToActionName<T>::name; };

	this->drawNew = []() { return ActionField<T>::drawField(); };
	this->drawExisting = [](Node* node, int index) { return ActionField<T>::drawField(node, index); };
	
	this->addStaticObjToNodeAtStep = [](id nodeId, index stepIndex) {
		Node* node = static_cast<Node*>(ModelEngineInterface::getNodeById(nodeId));
		if (node == nullptr) { return; }

		T obj = ActionField<T>::getStaticObj();

		if (ActionToType<T>::type == ActionAmount::SINGLE) {
			NodeBuilder{ node }.replaceAction<T>(stepIndex, obj);
		}
		else if (ActionToType<T>::type == ActionAmount::VECTOR) {
			NodeBuilder{ node }.addAction<T>(stepIndex, obj);
		}
	};

	this->performMove = [](ActionDragDropPayload payload) {
		Node* node = static_cast<Node*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return NodeBuilder{ node }.moveAction<T>(payload);
	};
	this->performCopy = [](ActionDragDropPayload payload) {
		Node* node = static_cast<Node*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return NodeBuilder{ node }.copyAction<T>(payload);
	};
	this->performSwap = [](ActionDragDropPayload payload) {
		Node* node = static_cast<Node*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return NodeBuilder{ node }.swapAction<T>(payload);
	};

	this->forceSwap = [](ActionDragDropPayload payload) {
		Node* node = static_cast<Node*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return NodeBuilder{ node }.forceSwap<T>(payload);
	};
	this->performDelete = [](ActionDragDropPayload payload) {
		Node* node = static_cast<Node*>(ModelEngineInterface::getNodeById(payload.m_nodeId));
		if (node == nullptr) { return false; }

		return NodeBuilder{ node }.deleteAction<T>(payload);
	};
	this->containsStep = [](id nodeId, index stepIndex) {
		Node* node = static_cast<Node*>(ModelEngineInterface::getNodeById(nodeId));
		if (node == nullptr) { return false; }

		return NodeBuilder{ node }.containsStep<T>(stepIndex);
	};

	this->equals =  [](ActionHelper helper) { return std::string(helper.getName()) == std::string(ActionToActionName<T>::name); };
}