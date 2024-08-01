#ifndef VN_NODE_BUILDER_H
#define VN_NODE_BUILDER_H

#include "node.h"

// Need to be careful with object splicing
class NodeBuilder {
protected:
	Node* m_nodeBase{ nullptr };

private:
	void nullCheck() {
		if (m_nodeBase == nullptr) {
			//throw 
		}
	}

public:
	NodeBuilder() = delete;

	NodeBuilder(Node* node) : 
		m_nodeBase{ node }
	{

	}

	//TODO: virtual destructor
	//virtual ~NodeBuilder();

private:
	//
	// Model File Loading Interface
	//

	void addParent(id parentId) { 
		if (m_nodeBase == nullptr) { return; }

		m_nodeBase->m_parents.insert(parentId);
	}

	void removeParent(id parentId) { 
		if (m_nodeBase == nullptr) { return; }

		m_nodeBase->m_parents.erase(parentId);
	}

	void addChild(id childId) {
		if (m_nodeBase == nullptr) { return; }
		
		m_nodeBase->m_children.insert(childId);
	}

	void removeChild(id childId) { 
		if (m_nodeBase == nullptr) { return; }
	
		m_nodeBase->m_children.erase(childId);
	}

public:
	//
	// Public Engine Interface
	//

	void link(Node* secondNode) {

	}

	void unlink(Node* secondNode) {

	}
};


#endif // VN_NODE_BUILDER_H
