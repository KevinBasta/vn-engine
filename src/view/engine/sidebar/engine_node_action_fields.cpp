
#include "id.h"

#include "model_engine_interface.h"
#include "chapter_node_types.h"

#include "action_type_mappers.h"

#include "engine_node_action_fields.h"

#include "texture_store.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <locale>
#include <codecvt>
#include <string>
#include <algorithm>

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;

static std::string addIdFromPtr(std::string name, void* ptr) { return name + std::to_string((unsigned long long)(void**)ptr); }
static std::string addIdFromPtr(std::wstring name, void* ptr) { 
	return myconv.to_bytes(name) + std::to_string((unsigned long long)(void**)ptr);
}

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

/*
* Common drawing units that are reused by several actions.
*/
bool drawTextureAndTextureStore(TextureIdentifier& texture) {
	static ImGuiComboFlags flags = 0;
	bool modified = false;

	ModelEngineInterface::TextureStoreMap& textureStores{ ModelEngineInterface::getTextureStoreMap() };

	// In the case that the texture store is invalid
	if (!textureStores.contains(texture.m_textureStoreId)) {
		texture.m_textureStoreId = (textureStores.begin())->first;
		// TODO: implicit that texture stores have 1 entry (0)??
	}

	TextureStore* currentStore{ textureStores.at(texture.m_textureStoreId).get() };
	const std::string& textureStoreName{ currentStore->getName() };

	if (ImGui::BeginCombo("Texture Store", textureStoreName.c_str(), flags)) {		
		for (auto iter{ textureStores.begin() }; iter != textureStores.end(); iter++) {
			const bool isSelected = (iter->first == texture.m_textureStoreId);
			if (ImGui::Selectable(iter->second.get()->getName().c_str(), isSelected)) {
				// avoid needless update
				if (texture.m_textureStoreId != iter->first) {
					texture.m_textureStoreId = iter->first;
					modified = true;
				}
			}

			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	std::pair<int, int> texturesRange{ currentStore->getTexturesRange() };
	std::string currentTextureIndex{ std::to_string(texture.m_textureIndex) };

	// TODO: update with preview image
	if (ImGui::BeginCombo("Texture", currentTextureIndex.c_str(), flags)) {
		for (int i{ texturesRange.first }; i < texturesRange.second; i++) {
			const bool isSelected = (i == texture.m_textureIndex);
			std::string currentIndexStr{ std::to_string(i) };
			if (ImGui::Selectable(currentIndexStr.c_str(), isSelected)) {
				// avoid needless update
				if (texture.m_textureIndex != i) {
					texture.m_textureIndex = i;
					modified = true;
				}
			}

			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return modified;
}


/*
* Background actions
*/

template<>
bool ActionField<ActionBackgroundTexture>::drawInternal(ActionBackgroundTexture* obj) {
	bool modified = false;
	if (obj == nullptr) { return modified; }

	std::string actionTitle{ "Background Texture##" + std::to_string((unsigned long long)(void**)obj) };

	bool isTreeOpen = ImGui::TreeNode(actionTitle.c_str());
	dragDropSourceSet();

	if (isTreeOpen) {
		modified |= drawTextureAndTextureStore(obj->m_texture);

		ImGui::TreePop();
	}

	return modified;
}


/*
* Sprite actions
*/

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
	std::string actionTitle{ textureStoreName + "::[" + std::to_string(obj->m_texture.m_textureIndex) + "]::" + toString(obj->m_property) +  "##" + std::to_string((unsigned long long)(void**)obj)};
	//std::string actionTitle{ "Sprite Property" };

	bool isTreeOpen = ImGui::TreeNode(actionTitle.c_str());
	dragDropSourceSet();

	if (isTreeOpen) {
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


/**
 * Text Action Types
 */

bool ActionField<ActionTextRender>::drawInternal(ActionTextRender* obj) {
	bool modified = false;
	
	return modified;
}

template<>
bool ActionField<ActionTextLine>::drawInternal(ActionTextLine* obj) {	
	bool modified = false;

	std::string actionTitle{ "ActionTextLine##" + std::to_string((unsigned long long)(void**)obj) };

	if (ImGui::TreeNode(actionTitle.c_str()))
	{

		// Character selection
		const ModelEngineInterface::CharacterMap& characterMap{ ModelEngineInterface::getCharacterMap() };
		
		// Set the object character to the first valid instance if it's not valid
		if (!characterMap.contains(obj->m_characterID)) {
			obj->m_characterID = characterMap.begin()->first;
		}
		
		id selected = obj->m_characterID;

		if (ImGui::BeginCombo("Character", myconv.to_bytes(characterMap.at(selected).get()->getName()).c_str(), NULL))
		{
			for (const auto& idCharacterPair : characterMap)
			{
				const bool isSelected = (idCharacterPair.first == selected);
				if (ImGui::Selectable(myconv.to_bytes(idCharacterPair.second.get()->getName()).c_str(), isSelected)) {
					obj->m_characterID = idCharacterPair.first;
					modified |= true;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		// TODO: decide if tabs should be allowed at all or not.
		//ImGui::DebugTextEncoding((const char*)u8"こんにちは");
		static ImGuiInputTextFlags flags{ 0 };
		flags |= ImGuiInputTextFlags_AllowTabInput;

		// Conver wstring to string, draw multiline input, convert string to wstring
		std::string convertedName = myconv.to_bytes(obj->m_line);
		modified |= ImGui::InputTextMultiline("Text Line", &(convertedName), ImVec2(0, 0), flags); //TODO: imgui id
		obj->m_line = myconv.from_bytes(convertedName);
		
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


/*
* Relations
*/

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


/*
* Direct setting actions (next chapter)
*/

bool ActionField<ActionSetNextChapter>::drawInternal(ActionSetNextChapter* obj) {
	bool modified = false;

	return modified;
}


/*
* Choice actions
*/

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