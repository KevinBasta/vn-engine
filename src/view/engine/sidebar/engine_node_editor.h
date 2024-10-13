#ifndef VN_ENGINE_NODE_EDITOR_H
#define VN_ENGINE_NODE_EDITOR_H

#include "window.h"
#include "context.h"
#include "state_subject.h"

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

public:
	VnEngineNodeEditor(StateSubject* stateSubject):
		m_stateSubject{ stateSubject }
	{

	}

	~VnEngineNodeEditor() {

	}


public:
	void draw() {
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
	}

};


#endif // VN_ENGINE_NODE_EDITOR_H