
#include "engine_node_editor.h"

#include <utility>

StateSubject* VnEngineNodeEditor::m_stateSubject{ nullptr };

const std::vector<VnEngineNodeEditor::ActionHelper> VnEngineNodeEditor::s_items {
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionBackgroundTexture>},
	
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionSpriteProperty>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionSpriteAnimationGeneric>},
	
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionTextRender>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionTextLine>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionTextOverrideSpeaker>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionTextOverrideColor>},
	
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionRelationModify>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionRelationSetNextNode>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionRelationSetNextChapter>},
	
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionSetNextChapter>},

	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionChoice>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionChoiceSetNextNode>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionChoiceModifyRelation>},
	VnEngineNodeEditor::ActionHelper{std::in_place_type<ActionChoiceSetNextChapter>},
};

int VnEngineNodeEditor::ComboActions::s_selectedIndex{ 0 };

