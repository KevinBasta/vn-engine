#ifndef VN_STATE_NEXT_NODE_H
#define VN_STATE_NEXT_NODE_H

#include "id.h"

#include <optional>
#include <vector>

class StateNextNode {
private:
	// Keep record of what choices were made for save file
	std::vector<int> m_nodeChoicesRecord{};
	
	std::optional<id> m_nextNodeId{};

public:
	void set(id nextNodeId) {
		m_nextNodeId.emplace(nextNodeId);
	}
	
	void reset() {
		m_nextNodeId.reset();
	}

	bool hasNextNodeId() { return m_nextNodeId.has_value(); }
	id getNextNodeId() { return m_nextNodeId.value(); }
	void recordNodeChildChoice(id nodeId) { m_nodeChoicesRecord.push_back(nodeId); }
};

#endif // VN_STATE_NEXT_NODE_H