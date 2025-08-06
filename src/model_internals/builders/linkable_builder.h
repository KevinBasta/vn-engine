#ifndef VN_LINKABLE_OBJECT_BUILDER_H
#define VN_LINKABLE_OBJECT_BUILDER_H

#include "linkable.h"

#include <set>
#include <vector>
#include <functional>

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

private:
	//
	// Model File Loading Interface
	//

	void addParent(id parentId) {
		if (m_linkableObject == nullptr) { return; }

		if (std::find(m_linkableObject->m_parents.begin(), m_linkableObject->m_parents.end(), parentId) == m_linkableObject->m_parents.end()) {
			m_linkableObject->m_parents.emplace_back(parentId);
		}
	}

	void removeParent(id parentId) {
		if (m_linkableObject == nullptr) { return; }

		auto iter = std::find(m_linkableObject->m_parents.begin(), m_linkableObject->m_parents.end(), parentId);

		if (iter != m_linkableObject->m_parents.end()) {
			m_linkableObject->m_parents.erase(iter);
		}
	}

	void addChild(id childId) {
		if (m_linkableObject == nullptr) { return; }

		if (std::find(m_linkableObject->m_children.begin(), m_linkableObject->m_children.end(), childId) == m_linkableObject->m_children.end()) {
			m_linkableObject->m_children.emplace_back(childId);
		}
	}

	void removeChild(id childId) {
		if (m_linkableObject == nullptr) { return; }

		auto iter = std::find(m_linkableObject->m_children.begin(), m_linkableObject->m_children.end(), childId);

		if (iter != m_linkableObject->m_children.end()) {
			m_linkableObject->m_children.erase(iter);
		}
	}

public:
	//
	// Public Engine Interface
	//

	void link(Linkable* secondLinkableObject) {
		if (m_linkableObject == nullptr || secondLinkableObject == nullptr) { std::cout << "LINKABLE BUILDER: ONE OF OBJECTS IS NULL" << std::endl; return;  }

		addChild(secondLinkableObject->getId());
		LinkableBuilder{ secondLinkableObject }.addParent(m_linkableObject->getId());
	}

	void unlink(Linkable* secondLinkableObject) {
		if (m_linkableObject == nullptr || secondLinkableObject == nullptr) { std::cout << "LINKABLE BUILDER: ONE OF OBJECTS IS NULL" << std::endl; return; }

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