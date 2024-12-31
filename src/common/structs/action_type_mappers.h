
#ifndef VN_ACTION_TYPE_MAPPERS_H
#define VN_ACTION_TYPE_MAPPERS_H

#include "chapter_node.h"
#include "chapter_node_types.h"
#include "chapter_node_builder.h"

#include "model_engine_interface.h"
#include "engine_drag_drop_payload.h"

enum ActionAmount {
	SINGLE,
	VECTOR
};

// Helper to avoid having to specify more data with the specific types T
struct ActionHelper {
public:
	template <class T>
	ActionHelper(std::in_place_type_t<T>);

	std::function<bool(ActionHelper)> equals;
	std::function<ActionAmount()> getType;
	std::function<void(ActionDragDropPayload)> performMove;
	std::function<bool()> drawNew;
	std::function<bool(ChapterNode* node, int index)> drawExisting;
	std::function<const char*()> getName;
};


#endif // VN_ACTION_TYPE_MAPPERS_H
