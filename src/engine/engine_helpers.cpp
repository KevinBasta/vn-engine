#include "engine_helpers.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "imgui_node_editor.h"
#include <chrono>
#include <string>

bool EngineToolTip::m_active{ false };
std::chrono::time_point<std::chrono::steady_clock> EngineToolTip::m_start{};
std::chrono::time_point<std::chrono::steady_clock> EngineToolTip::m_goal{};
std::string EngineToolTip::m_tip{ "" };
