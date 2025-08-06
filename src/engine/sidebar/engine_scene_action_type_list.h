
#ifndef VN_ENGINE_ACTION_TYPE_LIST_H
#define VN_ENGINE_ACTION_TYPE_LIST_H

#include "node_types.h"
#include "action_type_mappers.h"

#include <vector>
#include <utility>

const std::vector<ActionHelper> s_items{
	ActionHelper{std::in_place_type<ActionBackgroundTexture>},

	ActionHelper{std::in_place_type<ActionSpriteAllProperties>},
	ActionHelper{std::in_place_type<ActionSpriteAnimation>},

	ActionHelper{std::in_place_type<ActionTextLine>},
	ActionHelper{std::in_place_type<ActionTextOverrides>},

	ActionHelper{std::in_place_type<ActionRelationModify>},
	ActionHelper{std::in_place_type<ActionRelationSetNextNode>},
	ActionHelper{std::in_place_type<ActionRelationSetNextChapter>},

	ActionHelper{std::in_place_type<ActionSetNextChapter>},

	ActionHelper{std::in_place_type<ActionChoice>},
	ActionHelper{std::in_place_type<ActionChoiceSetNextNode>},
	ActionHelper{std::in_place_type<ActionChoiceModifyRelation>},
	ActionHelper{std::in_place_type<ActionChoiceSetNextChapter>},
};

#endif // VN_ENGINE_ACTION_TYPE_LIST_H
