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
protected:
	static StateSubject* m_stateSubject;

public:
	VnEngineNodeEditor(StateSubject* stateSubject) 
	{ 
		m_stateSubject = stateSubject;
	}

	~VnEngineNodeEditor() { }


private:
	static const std::vector<ActionHelper> s_items;

private:
	// Encapsulate the combo header of this section
	class ComboActions {
	private:
		static int s_selectedIndex;

		static std::string addComboId(const char* str) { return (std::string(str) + std::string("##ComboChoiceId")); }

	public:
		static bool drawSelection() {
			return (s_items.at(s_selectedIndex)).drawNew();
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
	public:
		static void drawNodeSteps() {
			if (m_stateSubject == nullptr) { return; }

			id nodeId{ m_stateSubject->getNodeId() };
			Node* nodeBase{ ModelEngineInterface::getNodeById(nodeId) };
			if (nodeBase == nullptr) { return; }

			// TODO: node objects to be brought into one object OR
			// do dynamic cast and handle failure of the cast?
			ChapterNode* node{ static_cast<ChapterNode*>(nodeBase) };

			for (index i{ 0 }; i < node->getTotalSteps(); i++) {
				drawStep(node, i);
			}
		}


		static void drawStep(ChapterNode* node, index stepIndex) {
			if (node == nullptr) { return; }

			std::string stepTitle{ "Step #" + std::to_string(stepIndex) };

			if (ImGui::TreeNode(stepTitle.c_str()))
			{
				for (int i{ 0 }; i < s_items.size(); i++)
				{
					bool modified = s_items.at(i).drawExisting(node, stepIndex);
					if (modified) { reloadStateToStep(stepIndex); }
				}

				ImGui::TreePop();
			}
		}

		static void reloadStateToStep(int stepIndex) {
			// If any of the fields (of the model) were moditifer, then update the state subject if 
			// currently viewing the step that was edited or a step after the step that was edited
			int currentStep{ m_stateSubject->getStepIndex() };

			if (stepIndex <= currentStep) {
				m_stateSubject->goToStepIndex(currentStep);
			}
		}
	};


private:
	// TODO: is this needed?
	void updateViewport() {

	}

public:
	void draw() {
		ComboActions::drawCombo();

		ImGui::Spacing();

		ComboActions::drawSelection();

		ImGui::Spacing();

		NodeSteps::drawNodeSteps();
	}
};


#endif // VN_ENGINE_NODE_EDITOR_H