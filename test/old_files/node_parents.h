#ifndef VN_NODE_PARENTS_H
#define VN_NODE_PARENTS_H

#include <list>

class Node;
class NodeBuilder;

class NodeParents {
private:
	friend class Node;
	friend class NodeBuilder;

	// For the purposes of freeing the child
	Node* m_owner{};

	// For the purpose of switching owner if current owner is detatched
	// Otherwise the entire graph would be traversed again
	std::list<Node*> m_parents{};

private:
	NodeParents(Node* currentNode);

	//
	// Functions to only ever be called by node_children
	//

	bool hasOwner() { return (m_owner != nullptr); }
	void addParent(Node* parent, Node* currentNode);
	void removeParent(Node* parent, Node* currentNode);


};

#endif // VN_NODE_PARENTS_H
