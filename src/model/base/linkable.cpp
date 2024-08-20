
#include "id.h"
#include "linkable.h"

#include <stdexcept>

id Linkable::getFirstChildId() {
	if (m_children.empty()) {
		throw std::length_error("Linkable: there are no children.");
	}

	return *(m_children.begin());
}

bool Linkable::hasChild(id childId) {
	return m_children.contains(childId);
}