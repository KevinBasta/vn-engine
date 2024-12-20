#ifndef VN_ENGINE_NODE_EDITOR_H
#define VN_ENGINE_NODE_EDITOR_H

#include "id.h"

#include "window.h"
#include "context.h"
#include "model_engine_interface.h"
#include "state_subject.h"
#include "node.h"
#include "chapter_node.h"
#include "chapter_node_builder.h"
#include "engine_node_action_fields.h"
#include "action_type_mappers.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <functional>
#include <utility>


class VnEngineNodeEditor {
private:
	// Encapsulate the combo header of this section
	class ComboActions {
	private:
		// Helper to avoid having to specify more data with the specific types T
		// TODO: this can be applicable to so much more, especially the model/state
		// and the helper structs they use to get pointers to functions and variables
		struct ActionHelper {
		public:
			template <class T>
			ActionHelper(std::in_place_type_t<T>)
				: draw([]() { return ActionField<T>::drawField(); }),
				  getName([]() { return ActionToActionName<T>::name; })
			{ }

			std::function<bool()> draw;
			std::function<const char* ()> getName;
		};

	private:
		static int s_selectedIndex;
		static const std::vector<ActionHelper> s_items;

		static std::string addComboId(const char* str) { return (std::string(str) + std::string("##ComboChoiceId")); }

	public:
		static bool drawSelection() {
			return (s_items.at(s_selectedIndex)).draw();
		}

		static void drawCombo() {
			if (ImGui::BeginCombo("Action", addComboId(s_items.at(s_selectedIndex).getName()).c_str(), NULL))
			{
				for (int i{ 0 }; i < s_items.size(); i++)
				{
					const bool isSelected = (i == s_selectedIndex);
					if (ImGui::Selectable(addComboId(s_items.at(i).getName()).c_str(), isSelected)) {
						s_selectedIndex = i;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
	};

	// Encapsulate the Steps of the node in this section
	class NodeSteps {

	};

private:
	StateSubject* m_stateSubject;

private:
	void updateViewport() {

	}

	void drawStepActions(ChapterNode* node, int index) {
		bool modified{ false };
		
		ActionField<ActionBackgroundTexture> backgroundTexture;
		modified = backgroundTexture.drawField(node, index);
		if (modified) { reloadStateToStep(index); }

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
		Node* nodeBase{ ModelEngineInterface::getNodeById(nodeId) };
		
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
		ComboActions::drawCombo();

		ImGui::Spacing();

		ComboActions::drawSelection();

		ImGui::Spacing();

		drawCurrentNodeSteps();

	}

	void reloadStateToStep(int stepIndex) {
		// If any of the fields (of the model) were moditifer, then update the state subject
		// If we are viewing the node that was edited at or past the point of the action // TO DO THIS LINE
	
		int currentStep{ m_stateSubject->getStepIndex() };
		
		if (currentStep <= stepIndex) {
			// TODO: need a new function to accumulate state up to current step in node
			m_stateSubject->reloadStateStep();
		}
	}

};


#endif // VN_ENGINE_NODE_EDITOR_H