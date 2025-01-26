
#ifndef VN_ENGINE_ACTION_TYPE_LIST_H
#define VN_ENGINE_ACTION_TYPE_LIST_H

#include "chapter_node_types.h"
#include "action_type_mappers.h"

#include <vector>
#include <utility>

const std::vector<ActionHelper> s_items{
	ActionHelper{std::in_place_type<ActionBackgroundTexture>},

	ActionHelper{std::in_place_type<ActionSpriteProperty>},
	ActionHelper{std::in_place_type<ActionSpriteAllProperties>},
	ActionHelper{std::in_place_type<ActionSpriteAnimationGeneric>},

	ActionHelper{std::in_place_type<ActionTextRender>},
	ActionHelper{std::in_place_type<ActionTextLine>},
	ActionHelper{std::in_place_type<ActionTextOverrideSpeaker>},
	ActionHelper{std::in_place_type<ActionTextOverrideColor>},

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
