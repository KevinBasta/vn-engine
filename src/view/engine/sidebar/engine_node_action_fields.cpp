
#include "action_type_mappers.h"

#include "engine_node_action_fields.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"


template<>
bool ActionField<ActionBackgroundTexture>::drawInternal() {
	bool modified = false;

	std::string actionTitle{ "Background Texture##" + std::to_string((unsigned long long)(void**)m_drawnObj) };

	if (ImGui::TreeNode(actionTitle.c_str()))
	{
		// TODO: make dropdown
		// get the ids from the model (engine interface)
		// limit the set of dragInt for texture index, and perhaps display names of texture stores and the individual textures too
		modified |= ImGui::DragInt("Texture Store Id: ", &(m_drawnObj->m_texture.m_textureStoreId), 1, 0, 10, "%d", ImGuiSliderFlags_WrapAround);
		modified |= ImGui::DragInt("Texture Index: ", &(m_drawnObj->m_texture.m_textureIndex), 1, 0, 10, "%d", ImGuiSliderFlags_WrapAround);

		ImGui::TreePop();
	}

	return modified;
}




template<>
bool ActionField<ActionSpriteProperty>::drawInternal() {
	bool modified = false;

	return modified;
}
template<>
bool ActionField<ActionSpriteAnimationGeneric>::drawInternal() {
	bool modified = false;

	return modified;
}




bool ActionField<ActionTextRender>::drawInternal() {
	bool modified = false;

	return modified;
}

template<>
bool ActionField<ActionTextLine>::drawInternal() {
	bool modified = false;

	std::string actionTitle{ "ActionTextLine##" + std::to_string((unsigned long long)(void**)m_drawnObj) };


	if (ImGui::TreeNode(actionTitle.c_str()))

		//if (ImGui::TreeNode("Multi-line Text Input"))
	{
		// Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
		// and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
		static char text[1024 * 16] =
			"test\n";

		static ImGuiInputTextFlags flags{ 0 };
		//HelpMarker("You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputTextMultiline() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example. (This is not demonstrated in imgui_demo.cpp because we don't want to include <string> in here)");
		ImGui::CheckboxFlags("ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::CheckboxFlags("AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
		ImGui::SameLine(); //HelpMarker("When _AllowTabInput is set, passing through the widget with Tabbing doesn't automatically activate it, in order to also cycling through subsequent widgets.");
		ImGui::CheckboxFlags("CtrlEnterForNewLine", &flags, ImGuiInputTextFlags_CtrlEnterForNewLine);

		ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
		
		ImGui::TreePop();
	}


	return modified;
}

bool ActionField<ActionTextOverrideSpeaker>::drawInternal() {
	bool modified = false;

	return modified;
}
bool ActionField<ActionTextOverrideColor>::drawInternal() {
	bool modified = false;

	return modified;
}




bool ActionField<ActionRelationModify>::drawInternal() {
	bool modified = false;

	return modified;
}
bool ActionField<ActionRelationSetNextNode>::drawInternal() {
	bool modified = false;

	return modified;
}
bool ActionField<ActionRelationSetNextChapter>::drawInternal() {
	bool modified = false;

	return modified;
}




bool ActionField<ActionSetNextChapter>::drawInternal() {
	bool modified = false;

	return modified;
}



bool ActionField<ActionChoice>::drawInternal() {
	bool modified = false;

	return modified;
}
bool ActionField<ActionChoiceSetNextNode>::drawInternal() {
	bool modified = false;

	return modified;
}
bool ActionField<ActionChoiceModifyRelation>::drawInternal() {
	bool modified = false;

	return modified;
}
bool ActionField<ActionChoiceSetNextChapter>::drawInternal() {
	bool modified = false;

	return modified;
}




/*
		if (ImGui::TreeNode("##Multi-line Text Input"))
		{
			// Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
			// and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
			static char text[1024 * 16] =
				"test\n";

			static ImGuiInputTextFlags flags{ 0 };
			//HelpMarker("You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputTextMultiline() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example. (This is not demonstrated in imgui_demo.cpp because we don't want to include <string> in here)");
			ImGui::CheckboxFlags("##ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			ImGui::CheckboxFlags("##AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine(); //HelpMarker("When _AllowTabInput is set, passing through the widget with Tabbing doesn't automatically activate it, in order to also cycling through subsequent widgets.");
			ImGui::CheckboxFlags("##CtrlEnterForNewLine", &flags, ImGuiInputTextFlags_CtrlEnterForNewLine);

			ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
			ImGui::TreePop();
		}

*/