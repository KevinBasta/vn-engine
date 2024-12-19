#ifndef VN_ENGINE_NODE_ACTION_FIELDS_H
#define VN_ENGINE_NODE_ACTION_FIELDS_H

#include "chapter_node_types.h"
#include "chapter_node_builder.h"
#include "chapter_node.h"

template <class T>
class ActionField {
private:
	static inline T m_obj{};
	T* m_drawnObj{ &m_obj };

	bool drawInternal();

public:
	T getObj() { return m_obj; }
	void clearObj() { m_obj = {}; }
	
	// Set m_drawnObj to m_obj and drawInternal()
	bool drawField() {
		m_drawnObj = &m_obj;
		
		return drawInternal();
	}

	// Set m_drawnObj to obj inside node at index and drawInternal()
	bool drawField(ChapterNode* node, int index) {
		bool modified = false;
		
		if (node == nullptr) { return modified; }
		m_drawnObj = ChapterNodeBuilder{ node }.getStepAction<T>(index);
		if (m_drawnObj == nullptr) { m_drawnObj = &m_obj; return modified; }
		
		modified = drawInternal();

		m_drawnObj = &m_obj;

		return modified;
	}
};

#endif // VN_ENGINE_NODE_ACTION_FIELDS_H