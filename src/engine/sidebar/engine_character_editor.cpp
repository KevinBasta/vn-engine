#include "engine_character_editor.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <glm/glm.hpp>
#include <string>

StateSubject* VnEngineCharacterEditor::m_stateSubject{ nullptr };

// Must give a 0 as input to avoid assignment of an id to non-real character
Character VnEngineCharacterEditor::m_addableCharacter{ 0 };
