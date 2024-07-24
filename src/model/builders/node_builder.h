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

private:
	//
	// Private Internal operations
	//

	friend class NodeChildren;
	friend class NodeParents;

	bool isOwned() { 
		if (m_nodeBase == nullptr) { return false; }
		
		return m_nodeBase->m_parents.hasOwner();
	}

	void addParent(Node* parent) { 
		if (m_nodeBase == nullptr) { return; }
		
		m_nodeBase->m_parents.addParent(parent, m_nodeBase);
	}

	void removeParent(Node* parent) { 
		if (m_nodeBase == nullptr) { return; }

		m_nodeBase->m_parents.removeParent(parent, m_nodeBase);
	}

	void makeReferencedChildOwned(Node* child) {
		if (m_nodeBase == nullptr) { return; }

		m_nodeBase->m_children.makeReferencedChildOwned(child);
	}

public:
	//
	// Public Interface (for engine and savefiles)
	//

	void addChild(Node* child) { 
		if (m_nodeBase == nullptr) { return; }
		
		m_nodeBase->m_children.addChild(m_nodeBase, child);
	}

	void removeChild(Node* child) { 
		if (m_nodeBase == nullptr) { return; }
	
		m_nodeBase->m_children.removeChild(m_nodeBase, child);
	}
};


#endif // VN_NODE_BUILDER_H
