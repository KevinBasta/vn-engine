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
		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		static int item_selected_idx = 0; // Here we store our selection data as an index.

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = items[item_selected_idx];

		if (ImGui::BeginCombo("combo 1", combo_preview_value, NULL))
		{
			for (int n = 0; n < 14; n++)
			{
				const bool is_selected = (item_selected_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_selected_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Spacing();
	}

};


#endif // VN_ENGINE_NODE_EDITOR_H