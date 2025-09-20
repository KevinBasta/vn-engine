
#include "id.h"

#include "model_engine_interface.h"
#include "node_types.h"

#include "action_type_mappers.h"

#include "engine_helpers.h"
#include "engine_scene_action_editor.h"

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
	// TODO: add id ptr to selectable
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
* Sprite actions
*/

// SECTION HELPERS

// Draw field and implement constraints depending on property
static bool drawSpritePropertyField(SpriteProperty property, float& value, bool& enabled) {
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
static bool drawSpritePropertyPicker(SpriteProperty property) {
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


static bool drawPropertyAnimationFields(auto& keyframes) {
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

		// Draw Sprite Property Picker
		ImGui::Spacing();
		ImGui::Text("Background Properties");

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






/**
 * Text Action Types
 */

// SECTION HELPERS
static bool characterCombo(id& characterId) {
	bool modified{ false };

	// Character selection
	const ModelEngineInterface::CharacterMap& characterMap{ ModelEngineInterface::getCharacterMap() };

	// Set the object character to the first valid instance if it's not valid
	if (characterMap.find(characterId) == characterMap.end()) {
		std::cout << "ERROR MAP DOES NOT CONTAIN CHARACTER ID" << std::endl;
		characterId = characterMap.begin()->first;
	}

	if (ImGui::BeginCombo(addIdFromPtr("###", &characterId).c_str(), myconv.to_bytes(characterMap.at(characterId).get()->getName()).c_str(), NULL))
	{
		for (const auto& idCharacterPair : characterMap)
		{
			const bool isSelected = (idCharacterPair.first == characterId);
			if (ImGui::Selectable(myconv.to_bytes(idCharacterPair.second.get()->getName()).c_str(), isSelected)) {
				characterId = idCharacterPair.first;
				modified |= true;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return modified;
}

template<>
bool ActionField<ActionTextLine>::drawInternal(ActionTextLine* obj) {	
	bool modified = false;

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionTextLine> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{		
		ImGui::Text("Narration: ");
		ImGui::SameLine();
		modified |= ImGui::Checkbox(addIdFromPtr("##TextLineAction", &obj->narration).c_str(), &obj->narration);

		// Disable character selection if narration is enabled
		if (obj->narration) { ImGui::BeginDisabled(); }

		// Character selection
		modified |= characterCombo(obj->characterId);
		ImGui::SameLine();
		ImGui::Text("Character");

		if (obj->narration) { ImGui::EndDisabled(); }

		// TODO: decide if tabs should be allowed at all or not.
		//ImGui::DebugTextEncoding((const char*)u8"こんにちは");
		static ImGuiInputTextFlags flags{ 0 };
		flags |= ImGuiInputTextFlags_AllowTabInput;

		// Conver wstring to string, draw multiline input, convert string to wstring
		std::string convertedLine = myconv.to_bytes(obj->line);
		modified |= ImGui::InputTextMultiline(addIdFromPtr("###", &obj->line).c_str(), &(convertedLine), ImVec2(0, 0), flags);
		obj->line = myconv.from_bytes(convertedLine);
		ImGui::SameLine();

		if (obj->narration) {
			ImGui::Text("Narration Line");
		}
		else {
			ImGui::Text("Dialogue Line");
		}
		
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

static bool relationModifier(ActionRelationModify& obj) {
	bool modified{ false };

	const ModelEngineInterface::RelationTypeMap& relationTypes{ ModelEngineInterface::getRelationTypesMap() };
	const ModelEngineInterface::CharacterMap& characterMap{ ModelEngineInterface::getCharacterMap() };

	if (characterMap.size() == 0) { ImGui::BeginDisabled(); }

	// Validate the current relation type id
	if (!relationTypes.contains(obj.relation.relationTypeId)) {
		obj.relation.relationTypeId = relationTypes.begin()->first;
	}

	ImGui::PushItemWidth(150.0f);
	ImGui::Text("Relation Of: ");
	ImGui::SameLine();
	modified |= characterCombo(obj.relation.characterOneId);

	ImGui::Text("With:        ");
	ImGui::SameLine();
	modified |= characterCombo(obj.relation.characterTwoId);

	ImGui::Text("Relationship Type: ");
	ImGui::SameLine();
	// TODO: relationTypes.at can fail when size is 0
	if (ImGui::BeginCombo(addIdFromPtr("###", &obj.relation.relationTypeId).c_str(), relationTypes.at(obj.relation.relationTypeId).c_str(), 0)) {
		for (auto iter{ relationTypes.begin() }; iter != relationTypes.end(); iter++) {
			const bool isSelected = (obj.relation.relationTypeId == iter->first);

			if (ImGui::Selectable(iter->second.c_str(), isSelected)) {
				// avoid needless update by only updating if a new item is selected
				if (obj.relation.relationTypeId != iter->first) {
					obj.relation.relationTypeId = iter->first;
					modified = true;
					ImGui::SetItemDefaultFocus();
				}
			}

		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();


	ImGui::PushItemWidth(100.0f);
	ImGui::Text("Modification Type: ");
	ImGui::SameLine();
	static std::vector<RelationModification> modificationChoices{ RelationModification::SET,RelationModification::ADD,RelationModification::SUBTRACT,RelationModification::MULTIPLY,RelationModification::DIVIDE };
	if (ImGui::BeginCombo(addIdFromPtr("###", &obj.modificationType).c_str(), toString(obj.modificationType).c_str(), 0)) {
		for (auto iter{ modificationChoices.begin() }; iter != modificationChoices.end(); iter++) {
			const bool isSelected = (obj.modificationType == *iter);

			if (ImGui::Selectable(toString(*iter).c_str(), isSelected)) {
				// avoid needless update by only updating if a new item is selected
				if (obj.modificationType != *iter) {
					obj.modificationType = *iter;
					modified = true;
					ImGui::SetItemDefaultFocus();
				}
			}

		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::Text("Value: ");
	ImGui::SameLine();
	int min{ obj.modificationType == RelationModification::DIVIDE ? 1 : 0 };
	modified |= ImGui::DragInt(addIdFromPtr("###", &obj.modificationValue).c_str(), &(obj.modificationValue), 1, min, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

	if (characterMap.size() == 0) { ImGui::EndDisabled(); }

	return modified;
}

bool ActionField<ActionRelationModify>::drawInternal(ActionRelationModify* obj) {
	bool modified = false;

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionRelationModify> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{
		modified |= relationModifier(*obj);

		ImGui::TreePop();
	}

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

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionChoice> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{
		ImGui::Text("Choice Style: ");
		ImGui::SameLine();

		ImGui::PushItemWidth(170.0f);
		static std::vector<ChoiceStyle> modificationChoices{ ChoiceStyle::LIST_TEXT_AREA, ChoiceStyle::LIST_MID_SCREEN };
		if (ImGui::BeginCombo(addIdFromPtr("###", &obj->style).c_str(), toString(obj->style).c_str(), 0)) {
			for (auto iter{ modificationChoices.begin() }; iter != modificationChoices.end(); iter++) {
				const bool isSelected = (obj->style == *iter);

				if (ImGui::Selectable(toString(*iter).c_str(), isSelected)) {
					// avoid needless update by only updating if a new item is selected
					if (obj->style != *iter) {
						obj->style = *iter;
						modified = true;
						ImGui::SetItemDefaultFocus();
					}
				}

			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		
		ImGui::Text("Choices: ");

		int i{ 0 };
		for (auto iter{ obj->choices.begin() }; iter != obj->choices.end(); iter++) {
			ImGui::Text((std::string("#") + std::to_string(i)).c_str());
			ImGui::SameLine();

			// Conver wstring to string, draw single line input, convert string to wstring
			static ImGuiInputTextFlags flags{ 0 };
			flags |= ImGuiInputTextFlags_AllowTabInput;

			std::string convertedName = myconv.to_bytes(*iter);
			ImGui::PushItemWidth(350.0f);
			modified |= ImGui::InputText(addIdFromPtr("###", &(*iter)).c_str(), &(convertedName), flags);
			ImGui::PopItemWidth();
			*iter = myconv.from_bytes(convertedName);

			// Button: delete a choice
			ImGui::SameLine();
			if (ImGui::Button(addIdFromPtr("Delete", &(*iter)).c_str(), ImVec2(150.0f, 0.0f))) {
				iter = obj->choices.erase(iter);
				modified = true;

				if (iter == obj->choices.end()) {
					break;
				}
			}

			i++;
		}

		// Button: add a choice
		if (ImGui::Button(addIdFromPtr("Add Choice", &obj->choices).c_str(), ImVec2(150.0f, 0.0f))) {
			obj->choices.emplace_back();
			modified = true;
		}
		
		ImGui::TreePop();
	}

	return modified;
}
bool ActionField<ActionChoiceSetNextNode>::drawInternal(ActionChoiceSetNextNode* obj) {
	bool modified = false;

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionChoiceSetNextNode> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{
		for (auto iter{ obj->nodeId.begin() }; iter != obj->nodeId.end(); iter++) {
			// Slider: which choice index
			bool choiceIndexModified{ false };
			ChoiceIndex tempIndex{ iter->first };

			ImGui::Text("Choice #");
			ImGui::SameLine();
			ImGui::PushItemWidth(50.0f);
			choiceIndexModified = ImGui::DragScalar(addIdFromPtr("###", &(iter->first)).c_str(), ImGuiDataType_U32, &tempIndex, 1, 0);
			ImGui::PopItemWidth();
			if (choiceIndexModified) {
				if (std::find_if(obj->nodeId.begin(), 
					obj->nodeId.end(), 
					[&tempIndex](auto& indexAndNodeId) { return (tempIndex == indexAndNodeId.first); }) == obj->nodeId.end()) 
				{
					iter->first = tempIndex;
					modified = true;
				}
			}

			// Slider: which node
			ImGui::SameLine();
			ImGui::Text("Node Id ");
			ImGui::SameLine();
			ImGui::PushItemWidth(50.0f);
			modified |= ImGui::DragScalar(addIdFromPtr("###", &(iter->second)).c_str(), ImGuiDataType_U32, &(iter->second), 1, 0);
			ImGui::PopItemWidth();

			// Button: delete an action
			ImGui::SameLine();
			if (ImGui::Button(addIdFromPtr("Delete", &(iter->second)).c_str(), ImVec2(75.0f, 0.0f))) {
				iter = obj->nodeId.erase(iter);
				modified = true;

				if (iter == obj->nodeId.end()) {
					break;
				}
			}
		}

		// Button: add an action
		if (ImGui::Button(addIdFromPtr("Add Choice Action", obj).c_str(), ImVec2(150.0f, 0.0f))) {
			int choiceIndex{ 0 };

			while (std::find_if(obj->nodeId.begin(), 
				obj->nodeId.end(), 
				[&choiceIndex](auto& indexAndNodeId) { return (choiceIndex == indexAndNodeId.first); }) != obj->nodeId.end()) 
			{
				choiceIndex++;
			}
		
			obj->nodeId.emplace_back(std::pair(choiceIndex, 0));
			modified = true;
		}

		ImGui::TreePop();
	}

	return modified;
}
bool ActionField<ActionChoiceSetNextChapter>::drawInternal(ActionChoiceSetNextChapter* obj) {
	bool modified = false;

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionChoiceSetNextChapter> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{
		for (auto iter{ obj->chapterId.begin() }; iter != obj->chapterId.end(); iter++) {
			// Slider: which choice index
			bool choiceIndexModified{ false };
			ChoiceIndex tempIndex{ iter->first };

			ImGui::Text("Choice #");
			ImGui::SameLine();
			ImGui::PushItemWidth(50.0f);
			choiceIndexModified = ImGui::DragScalar(addIdFromPtr("###", &(iter->first)).c_str(), ImGuiDataType_U32, &tempIndex, 1, 0);
			ImGui::PopItemWidth();
			if (choiceIndexModified) {
				if (std::find_if(obj->chapterId.begin(), 
					obj->chapterId.end(), 
					[&tempIndex](auto& indexAndNodeId) { return (tempIndex == indexAndNodeId.first); }) == obj->chapterId.end()) 
				{
					iter->first = tempIndex;
					modified = true;
				}
			}

			// Slider: which node
			ImGui::SameLine();
			ImGui::Text("Chapter Id ");
			ImGui::SameLine();
			ImGui::PushItemWidth(50.0f);
			modified |= ImGui::DragScalar(addIdFromPtr("###", &(iter->second)).c_str(), ImGuiDataType_U32, &(iter->second), 1, 0);
			ImGui::PopItemWidth();

			// Button: delete an action
			ImGui::SameLine();
			if (ImGui::Button(addIdFromPtr("Delete", &(iter->second)).c_str(), ImVec2(75.0f, 0.0f))) {
				iter = obj->chapterId.erase(iter);
				modified = true;

				if (iter == obj->chapterId.end()) {
					break;
				}
			}
		}

		// Button: add an action
		if (ImGui::Button(addIdFromPtr("Add Choice Action", obj).c_str(), ImVec2(150.0f, 0.0f))) {
			int choiceIndex{ 0 };

			while (std::find_if(obj->chapterId.begin(), 
				obj->chapterId.end(), 
				[&choiceIndex](auto& indexAndNodeId) { return (choiceIndex == indexAndNodeId.first); }) != obj->chapterId.end()) 
			{
				choiceIndex++;
			}

			obj->chapterId.emplace_back(std::pair(choiceIndex, 0));
			modified = true;
		}

		ImGui::TreePop();
	}

	return modified;
}
bool ActionField<ActionChoiceModifyRelation>::drawInternal(ActionChoiceModifyRelation* obj) {
	bool modified = false;

	std::string actionTitle{ ActionHelper{ std::in_place_type<ActionChoiceModifyRelation> }.getName() };

	bool isTreeOpen = ImGui::TreeNode(addIdFromPtr(actionTitle, obj).c_str());
	dragDropSourceSet(obj);

	if (isTreeOpen)
	{
		for (auto iter{ obj->relationModifications.begin() }; iter != obj->relationModifications.end(); iter++) {
			// Slider: which choice index
			bool choiceIndexModified{ false };
			ChoiceIndex tempIndex{ iter->first };

			ImGui::Text("Choice #");
			ImGui::SameLine();
			ImGui::PushItemWidth(50.0f);
			choiceIndexModified = ImGui::DragScalar(addIdFromPtr("###", &(iter->first)).c_str(), ImGuiDataType_U32, &tempIndex, 1, 0);
			ImGui::PopItemWidth();
			if (choiceIndexModified) {
				if (std::find_if(obj->relationModifications.begin(), 
					obj->relationModifications.end(), 
					[&tempIndex](auto& indexAndNodeId) { return (tempIndex == indexAndNodeId.first); }) == obj->relationModifications.end()) 
				{
					iter->first = tempIndex;
					modified = true;
				}
			}

			// Slider: which node
			ImGui::SameLine();
			ImGui::Text("Relation Modifications:");
			ImGui::Indent(30.0f);
			for (auto modification{ iter->second.begin() }; modification != iter->second.end(); modification++) {
				modified |= relationModifier(*modification);
				
				ImGui::Indent(30.0f);
				if (ImGui::Button(addIdFromPtr("Delete Relation Modification", &(*modification)).c_str(), ImVec2(250.0f, 0.0f))) {
					modification = iter->second.erase(modification);
					modified = true;

					if (modification == iter->second.end()) {
						break;
					}
				}
				ImGui::Unindent(30.0f);
				ImGui::Spacing();
			}

			if (ImGui::Button(addIdFromPtr("Add Relation Modification", &(iter->second)).c_str(), ImVec2(250.0f, 0.0f))) {
				iter->second.emplace_back();
				modified = true;
			}
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Unindent(30.0f);


			// Button: delete an action
			if (ImGui::Button(addIdFromPtr("Delete Choice Action", &(iter->second)).c_str(), ImVec2(200.0f, 0.0f))) {
				iter = obj->relationModifications.erase(iter);
				modified = true;

				if (iter == obj->relationModifications.end()) {
					break;
				}
			}
			ImGui::Spacing();
			ImGui::Spacing();
		}

		// Button: add an action
		if (ImGui::Button(addIdFromPtr("Add Choice Action", obj).c_str(), ImVec2(200.0f, 0.0f))) {
			int choiceIndex{ 0 };

			while (std::find_if(obj->relationModifications.begin(), 
				obj->relationModifications.end(), 
				[&choiceIndex](auto& indexAndNodeId) { return (choiceIndex == indexAndNodeId.first); }) != obj->relationModifications.end()) 
			{
				choiceIndex++;
			}

			obj->relationModifications.emplace_back(std::pair(choiceIndex, 0));
			modified = true;
		}

		ImGui::TreePop();
	}

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