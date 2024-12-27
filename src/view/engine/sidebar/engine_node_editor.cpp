
#include "engine_node_editor.h"
#include "action_type_mappers.h"

#include <utility>

StateSubject* VnEngineNodeEditor::m_stateSubject{ nullptr };

const std::vector<ActionHelper> VnEngineNodeEditor::s_items {
	ActionHelper{std::in_place_type<ActionBackgroundTexture>},
	
	ActionHelper{std::in_place_type<ActionSpriteProperty>},
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

int VnEngineNodeEditor::ComboActions::s_selectedIndex{ 0 };

