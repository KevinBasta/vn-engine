#ifndef VN_NODE_STRAY_H
#define VN_NODE_STRAY_H

#include "node.h"

class NodeStray {
private:
	// TODO: singleton 
	// TODO: free 
	static std::list<Node*> m_chapterNodes;

public:
	static void addStray(Node* stray) {
		m_chapterNodes.push_back(stray);
	}
};

#endif // VN_NODE_STRAY_H