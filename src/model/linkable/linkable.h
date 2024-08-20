#ifndef VN_LINKABLE_OBJECT_H
#define VN_LINKABLE_OBJECT_H

#include "id.h"

#include <set>

class LinkableBuilder;

class Linkable {
private: 
	friend class LinkableBuilder;

protected:
	id m_id{};

	// For the purposes of traversal
	std::set<id> m_parents{};
	std::set<id> m_children{};

public:
	Linkable() : m_id{ IdGenerator<Linkable>::getId() } {}
	virtual ~Linkable() {};

	id getId() { return m_id; }
	int getChildrenAmount() { return m_children.size(); }
	id getFirstChildId();
	bool hasChild(id childId);
};

#endif // VN_LINKABLE_OBJECT_H
