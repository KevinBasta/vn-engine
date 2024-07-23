#ifndef VN_NODE_BUILDER_H
#define VN_NODE_BUILDER_H

#include "node.h"

// Need to be careful with object splicing
class NodeBuilder {
protected:
	Node* m_nodeBase{ nullptr };

public:
	NodeBuilder() = delete;

	NodeBuilder(Node* node) : 
		m_nodeBase{ node }
	{

	}

	// Usecase: Connect a node to a different parent
	

	void setParent(Node* parent) { }
	void addChild(Node* child);
	void removeChild(int childId);



};


#endif // VN_NODE_BUILDER_H
