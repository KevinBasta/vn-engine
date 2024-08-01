
#ifndef NODE_CHILDREN_H
#define NODE_CHILDREN_H

#include <memory>
#include <vector>
#include <list>

class Node;

class NodeChildren {
private:
	// Still debating if should make owned chilred shared
	// and referenced children weak. Advantage is that
	// can check if weak children are null or not,
	// but unsure if that is useful or not yet
	std::list<std::unique_ptr<Node>> m_ownedChildren{};
	std::list<Node*> m_referencedChildren{};

	// A vector that stores all the owned and referenced 
	// children for indexing and viewing purposes
	std::vector<Node*> m_childrenViewer{};

private:
	void updateChildrenViewer();
	
	void addOwnedChild(Node* node);
	void addReferencedChild(Node* node);

	bool removeOwnedChild(Node* child);
	bool removeReferencedChild(Node* child);

public:
	NodeChildren();

	void addChild(Node* parent, Node* child);
	void removeChild(Node* parent, Node* child);
	void makeReferencedChildOwned(Node* child);

	int size();
	Node* getChildById(int childId);
	Node* getChildByIndex(int index);
	Node* operator[](int index);
	std::vector<Node*>& getChildrenView();
	
public: 
// TEMP FUNCTIONS ONLY FOR TESTING
	void print(int indentLevel = 0);

	std::list<std::unique_ptr<Node>>& getOwnedChildren() { return m_ownedChildren; }
	std::list<Node*>& getReferencedChildren() { return m_referencedChildren; }
};

#endif // NODE_CHILDREN_H