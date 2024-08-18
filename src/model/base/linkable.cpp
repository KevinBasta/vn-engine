
#include "id.h"
#include "linkable.h"

#include <stdexcept>

id Linkable::getFirstChildId() {
	if (m_children.size() == 0) {
		throw std::length_error("Linkable: there are no children.");
	}

	return *(m_children.begin());
}
