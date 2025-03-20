#ifndef VN_ENGINE_NODE_ACTION_FIELDS_H
#define VN_ENGINE_NODE_ACTION_FIELDS_H

#include "chapter_node_types.h"
#include "chapter_node_builder.h"
#include "chapter_node.h"

#include "engine_action_type_list.h"

#include "engine_drag_drop_payload.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <string>
#include <vector>
#include <algorithm>

enum ActionAmount;
struct ActionHelper;

template <class T>
class ActionField {
private:
	static inline T m_obj{};
	
	static bool drawInternal(T* obj);
	
	static inline ActionDragDropPayload m_dragDropPayload{};
	static void dragDropSourceSet(T* obj, bool overrideName = false, std::string newName = "") {
		// Function to be called after TreeNode function calls.

		// TODO: could make it so that if it's a new object, it's an easier way
		// to add an action to a step, currently just disabling dragging on the new (static) objects
		if (obj == &m_obj) { return; }

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ACTION_DRAG", &m_dragDropPayload, sizeof(ActionDragDropPayload));

			if (overrideName) {
				ImGui::Text("Dragging %s", newName.c_str());
			}
			else {
				ImGui::Text("Dragging %s", ActionHelper{ std::in_place_type<T> }.getName());
			}
			// Display preview (could be anything, e.g. when dragging an image we could decide to display
			// the filename and a small preview of the image, etc.)
			// if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }
			// if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
			// if (mode == Mode_Swap) { ImGui::Text("Swap %s", names[n]); }
			ImGui::EndDragDropSource();
		}
	}

public:
	static T getStaticObj() { return m_obj; }
	static void clearStaticObj() { m_obj = {}; }
	
	// Set m_drawnObj to m_obj and drawInternal()
	static bool drawField() {
		return drawInternal(&m_obj);
	}

	// Set m_drawnObj to obj inside node at index and drawInternal()
	static bool drawField(ChapterNode* node, int index) {
		bool modified = false;
		
		
		// Error checking and getting actions
		if (node == nullptr) { return modified; }
		std::vector<T>* objects = ChapterNodeBuilder{ node }.getStepActions<T>(index);
		if (objects == nullptr) { return modified; }
		if (objects->size() == 0) { return modified; }

		
		// Setting up drag drop source
		ActionHelper currentTypeHelper{ std::in_place_type<T> };
		auto typeEntryIter = find_if(s_items.begin(), s_items.end(), [&currentTypeHelper](ActionHelper listTypeHelper) {
			return currentTypeHelper.equals(listTypeHelper);
		});
		m_dragDropPayload.m_typeIndex = typeEntryIter - s_items.begin();
		
		m_dragDropPayload.m_nodeId = node->getId();
		m_dragDropPayload.m_sourceStepIndex = index;


		// Group actions together if they allow more than one of the same action type
		if (ActionHelper{ std::in_place_type<T> }.getType() == ActionAmount::VECTOR) {
			std::string actionTitle{ std::string(ActionHelper{std::in_place_type<T>}.getName()) + "##" + std::to_string((unsigned long long)(void**)objects) };
			
			bool isTreeOpen = ImGui::TreeNode(actionTitle.c_str());
			dragDropSourceSet(nullptr /* hacky way to pass the check and allow drag drop on action category */);

			if (isTreeOpen) {
				// Reuse the dragDropPayload for each sub action
				m_dragDropPayload.m_pickOne = true;
				
				for (auto& obj : *objects) {
					modified |= drawInternal(&obj);
					m_dragDropPayload.m_souceStepPickOneIndex++;
				}

				ImGui::TreePop();
			}
		}
		else /* The action type is ActionAmount::SINGLE */ {
			// TODO: if size of objects is > 1 throw error?
			for (auto& obj : *objects) {
				modified |= drawInternal(&obj);
			}
		}

		m_dragDropPayload = {};
		return modified;
	}
};

#endif // VN_ENGINE_NODE_ACTION_FIELDS_H