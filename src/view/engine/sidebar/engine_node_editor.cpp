
#include "engine_node_editor.h"

#include <utility>

int VnEngineNodeEditor::ComboActions::s_selectedIndex{ 0 };

const std::vector<VnEngineNodeEditor::ComboActions::ActionHelper> VnEngineNodeEditor::ComboActions::s_items {
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionBackgroundTexture>},
	
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionSpriteProperty>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionSpriteAnimationGeneric>},
	
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionTextRender>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionTextLine>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionTextOverrideSpeaker>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionTextOverrideColor>},
	
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionRelationModify>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionRelationSetNextNode>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionRelationSetNextChapter>},
	
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionSetNextChapter>},

	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionChoice>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionChoiceSetNextNode>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionChoiceModifyRelation>},
	VnEngineNodeEditor::ComboActions::ActionHelper{std::in_place_type<ActionChoiceSetNextChapter>},
};