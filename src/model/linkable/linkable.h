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

	id getId() const { return m_id; }
	int getParentsAmount() const { return m_parents.size(); }
	int getChildrenAmount() const { return m_children.size(); }
	id getFirstChildId() const;
	bool hasChild(id childId) const;
	const std::set<id>& getParentsSet() const { return m_parents; }
	const std::set<id>& getChildrenSet() const { return m_children; }
};

#endif // VN_LINKABLE_OBJECT_H
