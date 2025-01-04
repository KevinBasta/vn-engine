
#ifndef VN_ACTION_TYPE_MAPPERS_H
#define VN_ACTION_TYPE_MAPPERS_H

#include "chapter_node.h"
#include "chapter_node_types.h"
#include "chapter_node_builder.h"

#include "model_engine_interface.h"
#include "action_type_mappers_helpers.h"
#include "engine_drag_drop_payload.h"

#include <functional>

// Helper to avoid having to specify more data with the specific types T
struct ActionHelper {
public:
	template <class T>
	ActionHelper(std::in_place_type_t<T>);

	// Fetch data from static structs
	std::function<ActionAmount()> getType;
	std::function<const char*()> getName;
	
	// Draw fields in engine node editor
	std::function<bool()> drawNew;
	std::function<bool(ChapterNode* node, int index)> drawExisting;
 
	// Calling ChapterBuilder member functions
	std::function<void(id nodeId, index stepIndex)> addStaticObjToNodeAtStep;
	
	// Action functions
	std::function<bool(ActionDragDropPayload)> performMove;
	std::function<bool(ActionDragDropPayload)> performCopy;
	std::function<bool(ActionDragDropPayload)> performSwap;
	std::function<bool(ActionDragDropPayload)> performDelete;
	std::function<bool(ActionDragDropPayload)> forceSwap;
	
	std::function<bool(id nodeId, index stepIndex)> containsStep;
	
	// Other Helpers
	std::function<bool(ActionHelper)> equals;
};


#endif // VN_ACTION_TYPE_MAPPERS_H
