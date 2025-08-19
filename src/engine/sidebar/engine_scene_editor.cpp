
#include "engine_scene_editor.h"
#include "action_type_mappers.h"

#include "imgui.h"

#include <utility>

StateSubject* VnEngineSceneEditor::m_stateSubject{ nullptr };

int VnEngineSceneEditor::ComboActions::s_selectedComboIndex{ 0 };
int VnEngineSceneEditor::ComboActions::s_selectedStep{ 0 };

ActionDragMode VnEngineSceneEditor::NodeEditorOptions::m_mode{ ActionDragMode::DRAG_MOVE };
