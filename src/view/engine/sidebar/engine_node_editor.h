#ifndef VN_ENGINE_NODE_EDITOR_H
#define VN_ENGINE_NODE_EDITOR_H

#include "id.h"

#include "window.h"
#include "context.h"
#include "model_subject.h"
#include "state_subject.h"
#include "node.h"
#include "chapter_node.h"
#include "chapter_node_builder.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <GLFW/glfw3.h>

#include <string>

class VnEngineNodeEditor {
private:
	StateSubject* m_stateSubject;
	bool modified{ false };

private:
	void updateViewport() {

	}

	void check(bool fieldModified) {
		if (fieldModified) {
			modified = true;
		}
	}

	void sectionBackgroundTexture(ChapterNode* node, int index) {
		if (node == nullptr) { return; }

		ActionBackgroundTexture* action{ ChapterNodeBuilder{ node }.getStepAction<ActionBackgroundTexture>(index) };

		if (action == nullptr) { return; }


		std::string actionTitle{ "Background Texture##IdDifferentiator" + std::to_string(index) };

		if (ImGui::TreeNode(actionTitle.c_str()))
		{
			// TODO: make dropdown
			// get the ids from the model (engine interface)
			// limit the set of dragInt for texture index, and perhaps display names of texture stores and the individual textures too
			check(ImGui::DragInt("Texture Store Id: ", &(action->m_texture.m_textureStoreId), 1, 0, 10, "%d", ImGuiSliderFlags_WrapAround));
			check(ImGui::DragInt("Texture Index: ", &(action->m_texture.m_textureIndex), 1, 0, 10, "%d", ImGuiSliderFlags_WrapAround));
			
			ImGui::TreePop();
		}
	}

	void drawStepActions(ChapterNode* node, int index) {



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
		sectionBackgroundTexture(node, index);

	}

	void drawStep(ChapterNode* node, int index) {
		if (node == nullptr) { return; }

		std::string stepTitle{ "Step #" + std::to_string(index) };

		if (ImGui::TreeNode(stepTitle.c_str()))
		{
			drawStepActions(node, index);
			ImGui::TreePop();
		}

	}

	void drawCurrentNodeSteps() {
		if (m_stateSubject == nullptr) { return; }

		id nodeId{ m_stateSubject->getNodeId() };
		Node* nodeBase{ ModelSubject::getNodeById(nodeId) };
		
		if (nodeBase == nullptr) { return; }

		
		// TODO: node objects to be brought into one object OR
		// do dynamic cast and handle failure of the cast?
		ChapterNode* node{ static_cast<ChapterNode*>(nodeBase) };

		for (int i{ 0 }; i < node->m_totalSteps; i++) {
			drawStep(node, i);
		}
	}
	

public:
	VnEngineNodeEditor(StateSubject* stateSubject):
		m_stateSubject{ stateSubject }
	{

	}

	~VnEngineNodeEditor() {

	}


public:
	void draw() {
		modified = false;

		constexpr int itemNumb{ 3 };
		const char* items[itemNumb] = {
			"##foo",
			"ActionTextLine",
			"ActionTextOverrideColor"
		};
		static int itemSelectedIndex{ 0 };

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* comboPreviewValue = items[itemSelectedIndex];

		if (ImGui::BeginCombo("combo 1", comboPreviewValue, NULL))
		{
			for (int n = 0; n < itemNumb; n++)
			{
				const bool is_selected = (itemSelectedIndex == n);
				if (ImGui::Selectable(items[n], is_selected)) {
					itemSelectedIndex = n;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Spacing();

		if (itemSelectedIndex != 0) {
			
			switch (itemSelectedIndex)
			{
			case 1:
			{
				if (ImGui::BeginChild("ActionTextLine"))

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
					//ImGui::TreePop();
				}

				ImGui::EndChild();

				break;
			}
			default:
				break;
			}

		}
	
		drawCurrentNodeSteps();

		if (modified) {
			// TODO: need a new function to accumulate state up to current step in node
			m_stateSubject->notify();
		}
	}

};


#endif // VN_ENGINE_NODE_EDITOR_H