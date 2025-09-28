#ifndef VN_LINKABLE_OBJECT_BUILDER_H
#define VN_LINKABLE_OBJECT_BUILDER_H

#include "linkable.h"

#include <set>
#include <vector>
#include <functional>
#include <iostream>

class LinkableBuilder {
protected:
	Linkable* m_linkableObject{ nullptr };

private:
	void nullCheck() {
		if (m_linkableObject == nullptr) {
			//throw 
		}
	}

public: 
	LinkableBuilder() = delete;
	LinkableBuilder(Linkable* linkableObject) :
		m_linkableObject{ linkableObject }
	{
	}

	std::set<id>& getParents()  { return m_linkableObject->m_parents; }
	std::set<id>& getChildren() { return m_linkableObject->m_children; }

private:
	//
	// Model File Loading Interface
	//

	void addParent(id parentId) {
		if (m_linkableObject == nullptr) { return; }

		m_linkableObject->m_parents.insert(parentId);
	}

	void removeParent(id parentId) {
		if (m_linkableObject == nullptr) { return; }

		m_linkableObject->m_parents.erase(parentId);
	}

	void addChild(id childId) {
		if (m_linkableObject == nullptr) { return; }

		m_linkableObject->m_children.insert(childId);
	}

	void removeChild(id childId) {
		if (m_linkableObject == nullptr) { return; }

		m_linkableObject->m_children.erase(childId);
	}

public:
	//
	// Public Engine Interface
	//

	void link(Linkable* secondLinkableObject) {
		if (m_linkableObject == nullptr || secondLinkableObject == nullptr) { std::cout << "LINKABLE BUILDER: ONE OF OBJECTS IS NULL" << std::endl; return;  }

		// Link this as parent and secondLinkableObject as child
		addChild(secondLinkableObject->getId());
		LinkableBuilder{ secondLinkableObject }.addParent(m_linkableObject->getId());
	}

	void unlink(Linkable* secondLinkableObject) {
		if (m_linkableObject == nullptr || secondLinkableObject == nullptr) { std::cout << "LINKABLE BUILDER: ONE OF OBJECTS IS NULL" << std::endl; return; }

		// Unlink this as parent and secondLinkableObject as child
		if (secondLinkableObject != nullptr) {
			removeChild(secondLinkableObject->getId());
		}

		if (m_linkableObject != nullptr) {
			LinkableBuilder{ secondLinkableObject }.removeParent(m_linkableObject->getId());
		}
	}

public:
	// TODO: maybe an interface to save the node base class
	// portion to a file (same for loading) to be called by
	// sub classes?
};

#endif // VN_LINKABLE_OBJECT_BUILDER_H