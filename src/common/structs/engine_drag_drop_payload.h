#ifndef VN_ENGINE_DRAG_DROP_PAYLOAD_H
#define VN_ENGINE_DRAG_DROP_PAYLOAD_H

#include "id.h"
#include "index.h"

struct ActionDragDropPayload {
	int m_typeIndex{}; // For s_items

	id m_nodeId{};
	index m_sourceStepIndex{};
	index m_destinationStepIndex{};

	bool m_pickOne{ false };
	int m_souceStepPickOneIndex{};

	bool m_replace{ false };
};

#endif // VN_ENGINE_DRAG_DROP_PAYLOAD_H
