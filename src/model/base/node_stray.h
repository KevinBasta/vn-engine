#ifndef VN_NODE_STRAY_H
#define VN_NODE_STRAY_H

#include "node.h"

#include <list>
#include <memory>
#include <iostream>

class NodeStray {
private:
	static std::unique_ptr<NodeStray> m_instance;

	static void checkInstance() {
		if (m_instance.get() == nullptr) {
			m_instance = std::make_unique<NodeStray>();
		}
	}

private:
	std::list<Node*> m_chapterNodes{};

public:
	// TODO: constructor can be private
	NodeStray() : m_chapterNodes{} {

	}

	~NodeStray() {
		for (Node* node : m_chapterNodes) {
			std::cout << "deleting node stray: " << node->getId() << std::endl;
			delete node;
		}
	}

public:
	static void addStray(Node* stray) {
		checkInstance();

		NodeStray* instance{ m_instance.get() };

		instance->m_chapterNodes.push_back(stray);
	}

	static void removeStray(Node* stray) {
		checkInstance();

		NodeStray* instance{ m_instance.get() };

		instance->m_chapterNodes.remove(stray);
	}
};

#endif // VN_NODE_STRAY_H