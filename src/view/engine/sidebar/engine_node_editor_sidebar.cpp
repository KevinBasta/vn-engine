
#include "engine_node_editor_sidebar.h"
#include "action_type_mappers.h"

#include <utility>

StateSubject* VnEngineNodeEditor::m_stateSubject{ nullptr };

int VnEngineNodeEditor::ComboActions::s_selectedComboIndex{ 0 };
int VnEngineNodeEditor::ComboActions::s_selectedStep{ 0 };

ActionDragMode VnEngineNodeEditor::NodeEditorOptions::m_mode{ ActionDragMode::DRAG_MOVE };

bool VnEngineNodeEditor::NodeEditorToolTip::m_active{ false };
std::chrono::time_point<std::chrono::steady_clock> VnEngineNodeEditor::NodeEditorToolTip::m_start{};
std::chrono::time_point<std::chrono::steady_clock> VnEngineNodeEditor::NodeEditorToolTip::m_goal{};
std::string VnEngineNodeEditor::NodeEditorToolTip::m_tip{ "" };
