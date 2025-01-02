
#include "engine_node_editor_sidebar.h"
#include "action_type_mappers.h"

#include <utility>

StateSubject* VnEngineNodeEditor::m_stateSubject{ nullptr };

int VnEngineNodeEditor::ComboActions::s_selectedComboIndex{ 0 };
int VnEngineNodeEditor::ComboActions::s_selectedStep{ 0 };
ActionDragMode VnEngineNodeEditor::nodeEditorOptions::m_mode{ ActionDragMode::DRAG_MOVE };
