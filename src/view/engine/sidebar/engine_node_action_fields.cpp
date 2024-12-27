
#include "model_engine_interface.h"
#include "chapter_node_types.h"

#include "action_type_mappers.h"

#include "engine_node_action_fields.h"

#include "texture_store.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <string>

static std::string toString(enum class SpriteProperty property) {
	switch (property)	
	{
	case SpriteProperty::XPOS:
		return "XPOS";
	case SpriteProperty::YPOS:
		return "YPOS";
	case SpriteProperty::ZPOS:
		return "ZPOS";
	case SpriteProperty::SCALE:
		return "SCALE";
	case SpriteProperty::OPACITY:
		return "OPACITY";
	default:
		break;
	}

	return "NONE";
}

bool drawTextureAndTextureStore(TextureIdentifier& texture) {
	bool modified = false;

	ModelEngineInterface::TextureStoreMap& textureStores{ ModelEngineInterface::getTextureStoreMap() };

	// In the case that the texture store is invalid
	if (!textureStores.contains(texture.m_textureStoreId)) {
		texture.m_textureStoreId = (textureStores.begin())->first;
	}

	TextureStore* currentStore{ textureStores.at(texture.m_textureStoreId).get() };

	// Draw the texture store options
	const std::string& textureStoreName{ currentStore->getName() };
	modified |= ImGui::SliderInt("Texture Store", &(texture.m_textureStoreId), 1, textureStores.size(), textureStoreName.c_str()); // Use ImGuiSliderFlags_NoInput flag to disable CTRL+Click here.

	std::pair<int, int> texturesRange{ currentStore->getTexturesRange() };
	std::string currentTextureIndex{ std::to_string(texture.m_textureIndex) };
	modified |= ImGui::SliderInt("Texture", &(texture.m_textureIndex), texturesRange.first, texturesRange.second, currentTextureIndex.c_str());

	return modified;
}

template<>
bool ActionField<ActionBackgroundTexture>::drawInternal(ActionBackgroundTexture* obj) {
	bool modified = false;
	if (obj == nullptr) { return modified; }

	std::string actionTitle{ "Background Texture##" + std::to_string((unsigned long long)(void**)obj) };

	if (ImGui::TreeNode(actionTitle.c_str()))
	{
		modified |= drawTextureAndTextureStore(obj->m_texture);

		ImGui::TreePop();
	}

	return modified;
}




template<>
bool ActionField<ActionSpriteProperty>::drawInternal(ActionSpriteProperty* obj) {
	bool modified = false;
	if (obj == nullptr) { return modified; }

	ModelEngineInterface::TextureStoreMap& textureStores{ ModelEngineInterface::getTextureStoreMap() };
	
	// In the case that the texture store is invalid
	if (!textureStores.contains(obj->m_texture.m_textureStoreId)) {
		obj->m_texture.m_textureStoreId = (textureStores.begin())->first;
	}

	TextureStore* currentStore{ textureStores.at(obj->m_texture.m_textureStoreId).get() };
	const std::string& textureStoreName{ currentStore->getName() };
	//std::string actionTitle{ textureStoreName + "::[" + std::to_string(obj->m_texture.m_textureIndex) + "]::" + toString(obj->m_property) +  "##" + std::to_string((unsigned long long)(void**)obj)};
	std::string actionTitle{ "Sprite Property" };

	if (ImGui::TreeNode(actionTitle.c_str()))
	{
		modified |= drawTextureAndTextureStore(obj->m_texture);



		ImGui::TreePop();
	}
	
	return modified;
}
template<>
bool ActionField<ActionSpriteAnimationGeneric>::drawInternal(ActionSpriteAnimationGeneric* obj) {
	bool modified = false;

	return modified;
}




bool ActionField<ActionTextRender>::drawInternal(ActionTextRender* obj) {
	bool modified = false;

	return modified;
}

template<>
bool ActionField<ActionTextLine>::drawInternal(ActionTextLine* obj) {
	bool modified = false;

	std::string actionTitle{ "ActionTextLine##" + std::to_string((unsigned long long)(void**)obj) };


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

bool ActionField<ActionTextOverrideSpeaker>::drawInternal(ActionTextOverrideSpeaker* obj) {
	bool modified = false;

	return modified;
}
bool ActionField<ActionTextOverrideColor>::drawInternal(ActionTextOverrideColor* obj) {
	bool modified = false;

	return modified;
}




bool ActionField<ActionRelationModify>::drawInternal(ActionRelationModify* obj) {
	bool modified = false;

	return modified;
}
bool ActionField<ActionRelationSetNextNode>::drawInternal(ActionRelationSetNextNode* obj) {
	bool modified = false;

	return modified;
}
bool ActionField<ActionRelationSetNextChapter>::drawInternal(ActionRelationSetNextChapter* obj) {
	bool modified = false;

	return modified;
}




bool ActionField<ActionSetNextChapter>::drawInternal(ActionSetNextChapter* obj) {
	bool modified = false;

	return modified;
}



bool ActionField<ActionChoice>::drawInternal(ActionChoice* obj) {
	bool modified = false;

	return modified;
}
bool ActionField<ActionChoiceSetNextNode>::drawInternal(ActionChoiceSetNextNode* obj) {
	bool modified = false;

	return modified;
}
bool ActionField<ActionChoiceModifyRelation>::drawInternal(ActionChoiceModifyRelation* obj) {
	bool modified = false;

	return modified;
}
bool ActionField<ActionChoiceSetNextChapter>::drawInternal(ActionChoiceSetNextChapter* obj) {
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