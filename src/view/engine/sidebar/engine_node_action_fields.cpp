
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


/**
 * Text Action Types
 */

bool ActionField<ActionTextRender>::drawInternal(ActionTextRender* obj) {
	bool modified = false;
	
	return modified;
}

//struct ImGuiTextInputExtentionData
//{
//	std::wstring* m_string;
//
//	// FOR FUTURE EXTENTIONS
//	//ImGuiInputTextCallback  ChainCallback;
//	//void* ChainCallbackUserData;
//};
//
//static int InputTextCallback(ImGuiInputTextCallbackData* data)
//{
//	ImGuiTextInputExtentionData* user_data = (ImGuiTextInputExtentionData*)data->UserData;
//	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
//		// Resize string callback
//		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
//		std::wstring* str = user_data->m_string;
//		str->resize(data->BufTextLen);
//		data->Buf = (char*)str->c_str();
//	}
//	
//	// FOR FUTURE EXTENTIONS
//	//else if (user_data->ChainCallback) {
//		// Forward to user callback, if any
//		//data->UserData = user_data->ChainCallbackUserData;
//		//return user_data->ChainCallback(data);
//	//}
//
//	return 0;
//}
//
//
//bool InputTextMultiline(const char* label, std::wstring* str, const ImVec2& size, ImGuiInputTextFlags flags)
//{
//	ImGuiTextInputExtentionData data;
//	data.m_string = str;
//
//	return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &data);
//}



//struct InputTextCallback_UserData
//{
//	std::wstring* Str;
//	ImGuiInputTextCallback  ChainCallback;
//	void* ChainCallbackUserData;
//};
//
//static int InputTextCallback(ImGuiInputTextCallbackData* data)
//{
//	InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
//	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
//	{
//		// Resize string callback
//		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
//		std::wstring* str = user_data->Str;
//		str->resize(data->BufTextLen * 5);
//		
//		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
//		std::string converted = myconv.to_bytes(str->c_str());
//		
//		data->Buf = (char*)converted.c_str();
//	}
//
//	//else if (user_data->ChainCallback)
//	//{
//	//	// Forward to user callback, if any
//	//	data->UserData = user_data->ChainCallbackUserData;
//	//	return user_data->ChainCallback(data);
//	//}
//	return 0;
//}
//
//bool InputTextMultiline(const char* label, std::wstring* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
//{
//	IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
//	flags |= ImGuiInputTextFlags_CallbackResize;
//
//	InputTextCallback_UserData cb_user_data;
//	cb_user_data.Str = str;
//	cb_user_data.ChainCallback = callback;
//	cb_user_data.ChainCallbackUserData = user_data;
//
//
//	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
//	std::string converted = myconv.to_bytes(*str);
//
//	return ImGui::InputTextMultiline(label, (char*)converted.c_str(), converted.capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
//}

template<>
bool ActionField<ActionTextLine>::drawInternal(ActionTextLine* obj) {
	bool modified = false;

	std::string actionTitle{ "ActionTextLine##" + std::to_string((unsigned long long)(void**)obj) };






	if (ImGui::TreeNode(actionTitle.c_str()))
	{
		// UTF-8 test with Japanese characters
		// (Needs a suitable font? Try "Google Noto" or "Arial Unicode". See docs/FONTS.md for details.)
		// - From C++11 you can use the u8"my text" syntax to encode literal strings as UTF-8
		// - For earlier compiler, you may be able to encode your sources as UTF-8 (e.g. in Visual Studio, you
		//   can save your source files as 'UTF-8 without signature').
		// - FOR THIS DEMO FILE ONLY, BECAUSE WE WANT TO SUPPORT OLD COMPILERS, WE ARE *NOT* INCLUDING RAW UTF-8
		//   CHARACTERS IN THIS SOURCE FILE. Instead we are encoding a few strings with hexadecimal constants.
		//   Don't do this in your application! Please use u8"text in any language" in your application!
		// Note that characters values are preserved even by InputText() if the font cannot be displayed,
		// so you can safely copy & paste garbled characters into another application.

		//ImGui::DebugTextEncoding((const char*)u8"こんにちは");
		static ImGuiInputTextFlags flags{ 0 };
		flags |= ImGuiInputTextFlags_AllowTabInput;
		//flags |= ImGuiInputTextFlags_CallbackResize;
		
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
		std::string convertedName = myconv.to_bytes(obj->m_line);
		
		modified |= ImGui::InputTextMultiline("UTF-8 input", &(convertedName), ImVec2(0, 0), flags);
		
		obj->m_line = myconv.from_bytes(convertedName);
		
		//std::cout << "string: " << convertedName.capacity() << "wstring: " << obj->m_line.capacity() << std::endl;;
		
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