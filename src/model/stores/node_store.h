#ifndef VN_NODE_STRAY_H
#define VN_NODE_STRAY_H

#include "node.h"

#include <list>
#include <memory>
#include <iostream>

// TODO: When a new node is made add it to here, then remove it when it's attatched to a parent

class NodeStray {
private:
	static std::unique_ptr<NodeStray> m_instance;

	static void checkInstance() {
		if (m_instance.get() == nullptr) {
			NodeStray* stray{ new NodeStray };

			m_instance.reset(stray);
		}
	}

private:
	std::list<Node*> m_chapterNodes{};

	NodeStray() : m_chapterNodes{} {}

public:
	~NodeStray() {
		for (Node* node : m_chapterNodes) {
			std::cout << "deleting node stray: " << node->getId() << std::endl;
			delete node;
		}
	}

public:
	static void print() {
		checkInstance();

		NodeStray* instance{ m_instance.get() };

		std::cout << "Node strays" << std::endl;
		
		for (Node* node : instance->m_chapterNodes) {
			std::cout << node->getId() << std::endl;
		}
	}

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