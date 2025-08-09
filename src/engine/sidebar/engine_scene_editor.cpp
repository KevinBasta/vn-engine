
#include "engine_scene_editor.h"
#include "action_type_mappers.h"

#include "imgui.h"

#include <utility>

StateSubject* VnEngineSceneEditor::m_stateSubject{ nullptr };

int VnEngineSceneEditor::ComboActions::s_selectedComboIndex{ 0 };
int VnEngineSceneEditor::ComboActions::s_selectedStep{ 0 };

ActionDragMode VnEngineSceneEditor::NodeEditorOptions::m_mode{ ActionDragMode::DRAG_MOVE };

bool VnEngineSceneEditor::NodeEditorToolTip::m_active{ false };
std::chrono::time_point<std::chrono::steady_clock> VnEngineSceneEditor::NodeEditorToolTip::m_start{};
std::chrono::time_point<std::chrono::steady_clock> VnEngineSceneEditor::NodeEditorToolTip::m_goal{};
std::string VnEngineSceneEditor::NodeEditorToolTip::m_tip{ "" };
