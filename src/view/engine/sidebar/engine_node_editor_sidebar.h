#ifndef VN_ENGINE_NODE_EDITOR_SIDEBAR_H
#define VN_ENGINE_NODE_EDITOR_SIDEBAR_H

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
#include <chrono>

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
		static int s_selectedComboIndex;
		static index s_selectedStep;

		static std::string addComboId(const char* str) { return (std::string(str) + std::string("##ComboChoiceId")); }

	public:
		static void draw() {
			if (ImGui::TreeNodeEx(addComboId("Add Action").c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Spacing();

				drawCombo();

				ImGui::Spacing();

				drawSelection();

				ImGui::Spacing();

				addActionToStep();

				ImGui::Spacing();

				ImGui::TreePop();
			}
		}

		static bool addActionToStep() {
			// Decide the text displayed on the button
			std::string buttonName{};

			if ((s_items.at(s_selectedComboIndex)).getType() == ActionAmount::SINGLE) {
				// TODO: relying on deep error handling here to check that the node in state subject is valid. can check earlier
				if (s_items.at(s_selectedComboIndex).containsStep(m_stateSubject->getNodeId(), s_selectedStep)) { buttonName = "Replace"; }
				else { buttonName = "Set"; }
			}
			else { 
				buttonName = "Add"; 
			}
			
			// Used in the next two sections
			ChapterNode* node{ static_cast<ChapterNode*>(ModelEngineInterface::getNodeById(m_stateSubject->getNodeId())) };
			
			// Step Index picking
			ImGui::Text("Step Index:");
			
			ImGui::SameLine();
			
			ImGui::PushItemWidth(100.0f);
			if (ImGui::InputInt(addComboId("##InputSelectedStep").c_str(), &s_selectedStep)) {
				if (node == nullptr) {
					s_selectedStep = 0;
				}
				else {
					index totalSteps{ node->getTotalSteps() };
					if (s_selectedStep > totalSteps - 1) { s_selectedStep = totalSteps - 1; }
					if (s_selectedStep < 0) { s_selectedStep = 0; }
				}
			}
			ImGui::PopItemWidth();

			ImGui::SameLine();

			// Button submission
			if (ImGui::Button(addComboId(buttonName.c_str()).c_str(), ImVec2(150.0f, 0.0f))) {
				if (node != nullptr) {
					// TODO: need to handle null errors in both these funcitons.
					s_items.at(s_selectedComboIndex).addStaticObjToNodeAtStep(m_stateSubject->getNodeId(), s_selectedStep);
					NodeSteps::reloadStateToStep(s_selectedStep);
				}
			}

			return false;
		}

		static bool drawSelection() {
			return (s_items.at(s_selectedComboIndex)).drawNew();
		}

		static void drawCombo() {
			ImGui::Text("Action Type:");

			ImGui::SameLine();

			if (ImGui::BeginCombo("##InputComboType", addComboId(s_items.at(s_selectedComboIndex).getName()).c_str(), NULL))
			{
				for (int i{ 0 }; i < s_items.size(); i++)
				{
					const bool isSelected = (i == s_selectedComboIndex);
					if (ImGui::Selectable(addComboId(s_items.at(i).getName()).c_str(), isSelected)) {
						s_selectedComboIndex = i;
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

			bool isTreeOpen = ImGui::TreeNodeEx(stepTitle.c_str(), ImGuiTreeNodeFlags_Framed);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ACTION_DRAG"))
				{
					assert(payload->DataSize == sizeof(ActionDragDropPayload));

					bool reloadState{ false };
					ActionDragDropPayload payloadCast = *(const ActionDragDropPayload*)payload->Data;
					payloadCast.m_destinationStepIndex = stepIndex;

					if (NodeEditorOptions::getMode() == ActionDragMode::DRAG_MOVE)
					{
						reloadState = s_items.at(payloadCast.m_typeIndex).performMove(payloadCast);
						if (!reloadState) {
							NodeEditorToolTip::setTooltipFor(500, "Nothing to move here!");
						}
						else {
							NodeEditorToolTip::setTooltipFor(500, "Moved!");
						}
					}
					if (NodeEditorOptions::getMode() == ActionDragMode::DRAG_COPY)
					{
						reloadState = s_items.at(payloadCast.m_typeIndex).performCopy(payloadCast);
						if (!reloadState) {
							NodeEditorToolTip::setTooltipFor(500, "Nothing to copy here!");
						}
						else {
							NodeEditorToolTip::setTooltipFor(500, "Copied!");
						}
						
					}
					if (NodeEditorOptions::getMode() == ActionDragMode::DRAG_SWAP)
					{
						reloadState = s_items.at(payloadCast.m_typeIndex).performSwap(payloadCast);
						if (!reloadState) {
							NodeEditorToolTip::setTooltipFor(500, "Nothing to swap here!");
						}
						else {
							NodeEditorToolTip::setTooltipFor(500, "Swapped!");
						}
					}

					if (reloadState) {
						if (payloadCast.m_destinationStepIndex < payloadCast.m_sourceStepIndex) {
							reloadStateToStep(payloadCast.m_destinationStepIndex);
						}
						else {
							reloadStateToStep(payloadCast.m_sourceStepIndex);
						}
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

	class NodeEditorOptions {
	private:
		static ActionDragMode m_mode;
	
	public:
		static ActionDragMode getMode() { return m_mode; }

		static void draw() {

			ImGui::Text("Action Dragging Mode: ");

			ImGui::SameLine();

			if (ImGui::RadioButton("Move", m_mode == ActionDragMode::DRAG_MOVE)) { m_mode = ActionDragMode::DRAG_MOVE; } 
			
			ImGui::SameLine();
			
			if (ImGui::RadioButton("Copy", m_mode == ActionDragMode::DRAG_COPY)) { m_mode = ActionDragMode::DRAG_COPY; } 
			
			ImGui::SameLine();
			
			if (ImGui::RadioButton("Swap", m_mode == ActionDragMode::DRAG_SWAP)) { m_mode = ActionDragMode::DRAG_SWAP; }
			
			ImGui::Button("Delete", ImVec2(100, 100));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ACTION_DRAG"))
				{
					assert(payload->DataSize == sizeof(ActionDragDropPayload));

					bool reloadState{ false };
					ActionDragDropPayload payloadCast = *(const ActionDragDropPayload*)payload->Data;

					reloadState = s_items.at(payloadCast.m_typeIndex).performDelete(payloadCast);

					if (reloadState) {
						NodeSteps::reloadStateToStep(payloadCast.m_sourceStepIndex);
						NodeEditorToolTip::setTooltipFor(500, "Deleted!");
					}
				}
				ImGui::EndDragDropTarget();
			}

			// radio buttons for the drag type move, copy, swap
			// delete button that handles drag events
			// Add step button
		}
	};

	class NodeEditorToolTip {
	private:
		static bool m_active;
		static std::chrono::time_point<std::chrono::steady_clock> m_start;
		static std::chrono::time_point<std::chrono::steady_clock> m_goal;

		static std::string m_tip;
		
	public:
		// TODO: unsure if this should be kept or not
		static void setTooltipFor(int ms, std::string tip) {
			m_active = true;
			
			m_start = std::chrono::steady_clock::now();
			m_goal = m_start + std::chrono::milliseconds(ms);
			
			m_tip = tip;
		}

		static void draw() {
			if (m_active) {
				if (std::chrono::steady_clock::now() <= m_goal) {
					ImGui::SetTooltip(m_tip.c_str());
				}
				else {
					m_active = false;
				}
			}
		}
	};

private:
	// TODO: is this needed?
	void updateViewport() {

	}

public:
	void draw() {
		NodeEditorToolTip::draw();

		ComboActions::draw();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		NodeSteps::drawNodeSteps();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		NodeEditorOptions::draw();
	}
};


#endif // VN_ENGINE_NODE_EDITOR_SIDEBAR_H