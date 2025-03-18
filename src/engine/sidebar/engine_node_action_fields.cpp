
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

static std::string addIdFromPtr(std::string name, void* ptr) { return name + "##" + std::to_string((unsigned long long)(void**)ptr); }
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
	case SpriteProperty::ROTATION:
		return "ROTATION";
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
	if (!textureStores.contains(texture.textureStoreId)) {
		texture.textureStoreId = (textureStores.begin())->first;
		// TODO: implicit that texture stores have 1 entry (0)??
	}

	TextureStore* currentStore{ textureStores.at(texture.textureStoreId).get() };
	const std::string& textureStoreName{ currentStore->getName() };

	if (ImGui::BeginCombo("Texture Store", textureStoreName.c_str(), flags)) {		
		for (auto iter{ textureStores.begin() }; iter != textureStores.end(); iter++) {
			const bool isSelected = (iter->first == texture.textureStoreId);
			if (ImGui::Selectable(iter->second.get()->getName().c_str(), isSelected)) {
				// avoid needless update
				if (texture.textureStoreId != iter->first) {
					texture.textureStoreId = iter->first;
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
	std::string currentTextureIndex{ std::to_string(texture.textureIndex) };

	// TODO: update with preview image
	if (ImGui::BeginCombo("Texture", currentTextureIndex.c_str(), flags)) {
		for (int i{ texturesRange.first }; i < texturesRange.second; i++) {
			const bool isSelected = (i == texture.textureIndex);
			std::string currentIndexStr{ std::to_string(i) };
			if (ImGui::Selectable(currentIndexStr.c_str(), isSelected)) {
				// avoid needless update
				if (texture.textureIndex != i) {
					texture.textureIndex = i;
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

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionBackgroundTexture> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen) {
		modified |= drawTextureAndTextureStore(obj->texture);

		ImGui::TreePop();
	}

	return modified;
}






/*
* Sprite actions
*/

// SECTION HELPERS

// Draw field and implement constraints depending on property
bool drawSpritePropertyField(SpriteProperty property, float& value, bool& enabled) {
	bool modified{ false };
	// Min value, Max value, Step value
	float minMaxStep[3]{ -FLT_MAX, FLT_MAX, 0.5f };

	switch (property) {
	case SpriteProperty::XPOS:
	{
		minMaxStep[2] = 5.0f;
	}
	break;
	case SpriteProperty::YPOS:
	{
		minMaxStep[2] = 5.0f;
	}
	break;
	case SpriteProperty::ZPOS:
	{
		minMaxStep[2] = 1.0f;
	}
	break;
	case SpriteProperty::SCALE:
	{
		minMaxStep[2] = 0.005f;
	}
	break;
	case SpriteProperty::ROTATION:
	{
		minMaxStep[2] = 0.1f;
	}
	break;
	case SpriteProperty::OPACITY:
	{
		minMaxStep[0] = 0.0f;
		minMaxStep[1] = 1.0f;
		minMaxStep[2] = 0.005f;
	}
	break;
	}

	ImGui::Text(toString(property).c_str());
	ImGui::SameLine(130);
	bool tempModified = ImGui::DragFloat(addIdFromPtr("##SpritePropertyField", &value).c_str(), &value, minMaxStep[2], minMaxStep[0], minMaxStep[1], "%.3f", ImGuiSliderFlags_AlwaysClamp);
	if (enabled) { modified = tempModified; }	
	ImGui::SameLine();
	modified |= ImGui::Checkbox(addIdFromPtr("##SpriteCheckbox", &value).c_str(), &enabled);

	return modified;
}

// FOR PICKING FROM A DROPDOWN/COMBOBOX OF A LIST OF ELEMENTS
bool drawSpritePropertyPicker(SpriteProperty property) {
	bool modified{ false };

	// Draw Sprite Property Picker
	ImGui::Text("Sprite Property");
	static std::vector<SpriteProperty> propertyTypeChoices{ SpriteProperty::NONE,SpriteProperty::XPOS,SpriteProperty::YPOS,SpriteProperty::ZPOS,SpriteProperty::SCALE,SpriteProperty::OPACITY };

	if (ImGui::BeginCombo(addIdFromPtr("Property Type", &property).c_str(), toString(property).c_str(), 0)) {
		for (auto iter{ propertyTypeChoices.begin() }; iter != propertyTypeChoices.end(); iter++) {
			const bool isSelected = (property == *iter);

			if (ImGui::Selectable(toString(*iter).c_str(), isSelected)) {
				// avoid needless update
				if (property != *iter) {
					property = *iter;
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


// SECTION ACTIONS
template<>
bool ActionField<ActionSpriteAllProperties>::drawInternal(ActionSpriteAllProperties* obj) {
	bool modified = false;
	if (obj == nullptr) { return modified; }

	ModelEngineInterface::TextureStoreMap& textureStores{ ModelEngineInterface::getTextureStoreMap() };

	// In the case that the texture store is invalid
	if (!textureStores.contains(obj->texture.textureStoreId)) {
		obj->texture.textureStoreId = (textureStores.begin())->first;
	}

	TextureStore* currentStore{ textureStores.at(obj->texture.textureStoreId).get() };
	const std::string& textureStoreName{ currentStore->getName() };
	std::string actionTitle{ textureStoreName + "::[" + std::to_string(obj->texture.textureIndex) + "]" + "###" + std::to_string((unsigned long long)(void**)obj) };

	bool isTreeOpen = ImGui::TreeNode(actionTitle.c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen) {
		// Draw Texture Picker
		ImGui::Text("Texture");
		modified |= drawTextureAndTextureStore(obj->texture);

		// Draw Sprite Property Picker
		ImGui::Spacing();
		ImGui::Text("Sprite Properties");
		
		modified |= drawSpritePropertyField(SpriteProperty::XPOS, obj->xpos, obj->xposEnabled);
		modified |= drawSpritePropertyField(SpriteProperty::YPOS, obj->ypos, obj->yposEnabled);
		modified |= drawSpritePropertyField(SpriteProperty::ZPOS, obj->zpos, obj->zposEnabled);
		modified |= drawSpritePropertyField(SpriteProperty::SCALE, obj->scale, obj->scaleEnabled);
		modified |= drawSpritePropertyField(SpriteProperty::ROTATION, obj->rotation, obj->rotationEnabled);
		modified |= drawSpritePropertyField(SpriteProperty::OPACITY, obj->opacity, obj->opacityEnabled);

		ImGui::TreePop();
	}
	return modified;
}


bool drawPropertyAnimationFields(auto& keyframes) {
	bool modified{ false };

	int i{ 0 };
	for (auto iter{ keyframes.begin() }; iter != keyframes.end(); iter++) {
		ImGui::Text((std::string("#") + std::to_string(i)).c_str());
		ImGui::SameLine();

		ImGui::PushItemWidth(100.0f);
		// TODO: disallow negative
		modified |= ImGui::DragFloat(addIdFromPtr("Value", &iter->value).c_str(), &(iter->value), 0.1f);
		ImGui::SameLine();
		modified |= ImGui::DragFloat(addIdFromPtr("Seconds", &iter->transitionSeconds).c_str(), &(iter->transitionSeconds), 0.1f);
		ImGui::PopItemWidth();

		ImGui::SameLine();
		if (ImGui::Button(addIdFromPtr("Delete", &iter->value).c_str(), ImVec2(150.0f, 0.0f))) {
			iter = keyframes.erase(iter);
			modified = true;

			if (iter == keyframes.end()) {
				break;
			}
		}

		i++;
	}

	// Button submission
	if (ImGui::Button(addIdFromPtr("Add Step", &keyframes).c_str(), ImVec2(150.0f, 0.0f))) {
		keyframes.emplace_back();
		modified = true;
	}

	return modified;
}

template<>
bool ActionField<ActionSpriteAnimation>::drawInternal(ActionSpriteAnimation* obj) {
	bool modified = false;
	if (obj == nullptr) { return modified; }

	ModelEngineInterface::TextureStoreMap& textureStores{ ModelEngineInterface::getTextureStoreMap() };

	// In the case that the texture store is invalid
	if (!textureStores.contains(obj->texture.textureStoreId)) {
		obj->texture.textureStoreId = (textureStores.begin())->first;
	}

	TextureStore* currentStore{ textureStores.at(obj->texture.textureStoreId).get() };
	const std::string& textureStoreName{ currentStore->getName() };
	std::string actionTitle{ textureStoreName + "::[" + std::to_string(obj->texture.textureIndex) + "]::###" + std::to_string((unsigned long long)(void**)obj) };

	bool isTreeOpen = ImGui::TreeNode(actionTitle.c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen) {
		// Draw Texture Picker
		ImGui::Text("Texture");
		modified |= drawTextureAndTextureStore(obj->texture);

		ImGui::Separator();
		
		ImGui::Spacing();
		ImGui::Text("X POS");
		ImGui::SameLine();
		modified |= ImGui::Checkbox(addIdFromPtr("##SpriteCheckbox", &obj->xposEnabled).c_str(), &obj->xposEnabled);
		modified |= drawPropertyAnimationFields(obj->xpos);

		ImGui::Spacing();
		ImGui::Text("Y POS");
		ImGui::SameLine();
		modified |= ImGui::Checkbox(addIdFromPtr("##SpriteCheckbox", &obj->yposEnabled).c_str(), &obj->yposEnabled);
		modified |= drawPropertyAnimationFields(obj->ypos);

		ImGui::Spacing();
		ImGui::Text("Z POS");
		ImGui::SameLine();
		modified |= ImGui::Checkbox(addIdFromPtr("##SpriteCheckbox", &obj->zposEnabled).c_str(), &obj->zposEnabled);
		modified |= drawPropertyAnimationFields(obj->zpos);

		ImGui::Spacing();
		ImGui::Text("SCALE");
		ImGui::SameLine();
		modified |= ImGui::Checkbox(addIdFromPtr("##SpriteCheckbox", &obj->scaleEnabled).c_str(), &obj->scaleEnabled);
		modified |= drawPropertyAnimationFields(obj->scale);

		ImGui::Spacing();
		ImGui::Text("ROTATION");
		ImGui::SameLine();
		modified |= ImGui::Checkbox(addIdFromPtr("##SpriteCheckbox", &obj->rotationEnabled).c_str(), &obj->rotationEnabled);
		modified |= drawPropertyAnimationFields(obj->rotation);

		ImGui::Spacing();
		ImGui::Text("OPACITY");
		ImGui::SameLine();
		modified |= ImGui::Checkbox(addIdFromPtr("##SpriteCheckbox", &obj->opacityEnabled).c_str(), &obj->opacityEnabled);
		modified |= drawPropertyAnimationFields(obj->opacity);

		ImGui::TreePop();
	}

	return modified;
}


/**
 * Text Action Types
 */
template<>
bool ActionField<ActionTextLine>::drawInternal(ActionTextLine* obj) {	
	bool modified = false;

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionTextLine> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{
		// Character selection
		const ModelEngineInterface::CharacterMap& characterMap{ ModelEngineInterface::getCharacterMap() };
		
		// Set the object character to the first valid instance if it's not valid
		if (!characterMap.contains(obj->characterId)) {
			obj->characterId = characterMap.begin()->first;
		}
		
		id selected = obj->characterId;

		if (ImGui::BeginCombo("Character", myconv.to_bytes(characterMap.at(selected).get()->getName()).c_str(), NULL))
		{
			for (const auto& idCharacterPair : characterMap)
			{
				const bool isSelected = (idCharacterPair.first == selected);
				if (ImGui::Selectable(myconv.to_bytes(idCharacterPair.second.get()->getName()).c_str(), isSelected)) {
					obj->characterId = idCharacterPair.first;
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
		std::string convertedName = myconv.to_bytes(obj->line);
		modified |= ImGui::InputTextMultiline("Text Line", &(convertedName), ImVec2(0, 0), flags); //TODO: imgui id
		obj->line = myconv.from_bytes(convertedName);
		
		ImGui::TreePop();
	}

	return modified;
}



bool ActionField<ActionTextOverrides>::drawInternal(ActionTextOverrides* obj) {
	bool modified = false;

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionTextOverrides> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{
		// Speaker text apprering override
		bool renderModified = false;

		ImGui::Text("Render Dialogue: ");

		modified |= ImGui::Checkbox(addIdFromPtr("##render", obj).c_str(), &(obj->renderEnabled));
		ImGui::SameLine();
		
		if (!obj->renderEnabled) { ImGui::BeginDisabled(); }
		int textRenderState{ (obj->render) ? 1 : 0 };
		ImGui::PushItemWidth(150.0f);
		renderModified |= ImGui::SliderInt(addIdFromPtr("##renderSlider", obj).c_str(), &(textRenderState), 0, 1, (obj->render) ? "YES" : "NO");
		ImGui::PopItemWidth();
		if (renderModified) { 
			(textRenderState == 1) ? obj->render = true : obj->render = false; 
			if (obj->renderEnabled) { modified = true; }
		};
		if (!obj->renderEnabled) { ImGui::EndDisabled(); }



		// Speaker text color override
		bool colorModified = false;
		ImGuiColorEditFlags colorFlags = 0;

		ImGui::Text("Override Color: ");

		modified |= ImGui::Checkbox(addIdFromPtr("##color", obj).c_str(), &(obj->colorEnabled));
		ImGui::SameLine();

		if (!obj->colorEnabled) { ImGui::BeginDisabled(); }
		ImVec4 color = ImVec4(obj->color.r, obj->color.g, obj->color.b, 1.0f);
		colorModified |= ImGui::ColorEdit3(addIdFromPtr("##colorPicker", obj).c_str(), (float*)&color, colorFlags);
		obj->color = { color.x, color.y, color.z };
		
		if (colorModified && obj->colorEnabled) { modified = true; }
		if (!obj->colorEnabled) { ImGui::EndDisabled(); }



		// Speaker name override
		bool nameModified = false;
		static ImGuiInputTextFlags textFlags{ 0 };

		ImGui::Text("Override Speaker: ");

		modified |= ImGui::Checkbox(addIdFromPtr("##speaker", obj).c_str(), &(obj->speakerEnabled));
		ImGui::SameLine();
		
		if (!obj->speakerEnabled) { ImGui::BeginDisabled(); }
		std::string convertedName = myconv.to_bytes(obj->speaker);
		ImGui::PushItemWidth(150.0f);
		nameModified |= ImGui::InputText(addIdFromPtr("##speakerName", obj).c_str(), &(convertedName), textFlags);
		ImGui::PopItemWidth();
		obj->speaker = myconv.from_bytes(convertedName);

		if (nameModified && obj->speakerEnabled) { modified = true; }
		if (!obj->speakerEnabled) { ImGui::EndDisabled(); }

		ImGui::TreePop();
	}

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