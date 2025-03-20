#ifndef VN_ENGINE_NODE_MANAGER_H
#define VN_ENGINE_NODE_MANAGER_H

#include "id.h"
#include "node.h"

#include <memory>
#include <unordered_map>

class EngineNodeManager {
private:
	std::unordered_map<id, std::unique_ptr<Node>> m_nodes{};

public:
	EngineNodeManager() {}
	~EngineNodeManager() {}

	Node* get(id nodeId) {
		auto iter{ m_nodes.find(nodeId) };

		if (iter == m_nodes.end()) {
			// TODO: attempt to read the file
			// BUT should already covered by prefetcher on a diff thread,
			// so maybe check a vector containing the nodes currently loading
			return nullptr;
		}
		else {
			// return a pointer to the node
			return iter->second.get();
		}
	}

	void add(Node* node) {
		if (node == nullptr) {
			return;
		}

		m_nodes[node->getId()] = std::unique_ptr<Node>(node);
	}


};


#endif // VN_ENGINE_NODE_MANAGER_H