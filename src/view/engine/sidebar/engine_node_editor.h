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

#include "action_type_mappers.h"
#include "engine_node_action_fields.h"
#include "engine_action_type_list.h"

#include "engine_drag_drop_payload.h"

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

enum ActionDragMode {
	DRAG_COPY,
	DRAG_MOVE,
	DRAG_SWAP
};

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

			bool isTreeOpen = ImGui::TreeNode(stepTitle.c_str());

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ACTION_DRAG"))
				{
					assert(payload->DataSize == sizeof(ActionDragDropPayload));
					ActionDragDropPayload payloadCast = *(const ActionDragDropPayload*)payload->Data;
					payloadCast.m_destinationStepIndex = stepIndex;

					ActionDragMode mode = ActionDragMode::DRAG_MOVE;
					if (mode == ActionDragMode::DRAG_MOVE)
					{
						s_items.at(payloadCast.m_typeIndex).performMove(payloadCast);
					}
					if (mode == ActionDragMode::DRAG_COPY)
					{
						// TODO	
					}
					if (mode == ActionDragMode::DRAG_SWAP)
					{
						// TODO
					}


					if (payloadCast.m_destinationStepIndex < payloadCast.m_sourceStepIndex) {
						reloadStateToStep(payloadCast.m_destinationStepIndex);
					}
					else {
						reloadStateToStep(payloadCast.m_sourceStepIndex);
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (isTreeOpen)
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