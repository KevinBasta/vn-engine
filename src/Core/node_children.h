
#ifndef NODE_CHILDREN_H
#define NODE_CHILDREN_H

#include <memory>
#include <vector>

class Node;

class NodeChildren {
private:
	// Still debating if should make owned chilred shared
	// and referenced children weak. Advantage is that
	// can check if weak children are null or not,
	// but unsure if that is useful or not yet
	std::vector<std::unique_ptr<Node>> m_ownedChildren{};
	std::vector<Node*> m_referencedChildren{};

	// A vector that stores all the owned and referenced 
	// children for indexing and viewing purposes
	std::vector<Node*> m_childrenViewer{};

private:
	void updateChildrenViewer();
	void addOwnedChild(Node* node);
	void addReferencedChild(Node* node);

public:
	NodeChildren();

	int size();
	void addChild(Node* parent, Node* child);
	Node* getChildByIndex(int index);
	Node* operator[](int index);
	std::vector<Node*>& getChildrenView();

	//Node* getChildById(int id);
};

#endif // NODE_CHILDREN_H