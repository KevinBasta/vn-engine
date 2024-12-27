#ifndef VN_ENGINE_NODE_ACTION_FIELDS_H
#define VN_ENGINE_NODE_ACTION_FIELDS_H

#include "chapter_node_types.h"
#include "chapter_node_builder.h"
#include "chapter_node.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <string>

enum ActionAmount;
struct ActionHelper;

template <class T>
class ActionField {
private:
	static inline T m_obj{};
	static bool drawInternal(T* obj);

public:
	static T getObj() { return m_obj; }
	static void clearObj() { m_obj = {}; }
	
	// Set m_drawnObj to m_obj and drawInternal()
	static bool drawField() {
		return drawInternal(&m_obj);
	}

	// Set m_drawnObj to obj inside node at index and drawInternal()
	static bool drawField(ChapterNode* node, int index) {
		bool modified = false;
		
		if (node == nullptr) { return modified; }
		std::vector<T>& objects = ChapterNodeBuilder{ node }.getStepActions<T>(index);
		if (objects.size() == 0) { return modified; }

		// Group actions together if they allow more than one of the same action type
		if (ActionHelper{ std::in_place_type<T> }.getType() == ActionAmount::VECTOR) {
			std::string actionTitle{ std::string(ActionHelper{std::in_place_type<T>}.getName()) + "##" + std::to_string((unsigned long long)(void**)&objects) };
			
			if (ImGui::TreeNode(actionTitle.c_str())) {
				for (auto& obj : objects) {
					modified |= drawInternal(&obj);
				}

				ImGui::TreePop();
			}
		}
		else {
			for (auto& obj : objects) {
				modified |= drawInternal(&obj);
			}
		}

		return modified;
	}
};

#endif // VN_ENGINE_NODE_ACTION_FIELDS_H