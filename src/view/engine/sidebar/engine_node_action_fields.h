#ifndef VN_ENGINE_NODE_ACTION_FIELDS_H
#define VN_ENGINE_NODE_ACTION_FIELDS_H

#include "chapter_node_types.h"
#include "chapter_node_builder.h"
#include "chapter_node.h"

template <class T>
class ActionField {
private:
	static inline T m_obj{};
	static bool drawInternal(T* obj);

public:
	static T getObj() { return m_obj; }
	static void clearObj() { m_obj = {}; }
	
	// Set m_drawnObj to m_obj and drawInternal()
	static bool drawField() {
		return drawInternal(&m_obj);
	}

	// Set m_drawnObj to obj inside node at index and drawInternal()
	static bool drawField(ChapterNode* node, int index) {
		bool modified = false;
		
		if (node == nullptr) { return modified; }
		std::vector<T>& objects = ChapterNodeBuilder{ node }.getStepActions<T>(index);
		
		for (auto& obj : objects) {
			modified |= drawInternal(&obj);
		}

		return modified;
	}
};

#endif // VN_ENGINE_NODE_ACTION_FIELDS_H