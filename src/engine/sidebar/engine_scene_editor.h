#ifndef VN_ENGINE_NODE_EDITOR_SIDEBAR_H
#define VN_ENGINE_NODE_EDITOR_SIDEBAR_H

#include "id.h"

#include "window.h"
#include "context.h"

#include "model_engine_interface.h"
#include "state_subject.h"

#include "node.h"
#include "node_builder.h"

#include "action_type_mappers.h"
#include "engine_helpers.h"
#include "engine_scene_action_editor.h"
#include "engine_scene_action_type_list.h"
#include "linkable_builder.h"

#include "engine_drag_drop_payload.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <GLFW/glfw3.h>

#include <string>
#include <set>
#include <functional>
#include <utility>
#include <chrono>
#include <locale>
#include <codecvt>
#include <algorithm>

enum ActionDragMode {
	DRAG_COPY,
	DRAG_MOVE,
	DRAG_SWAP
};

class VnEngineSceneEditor {
protected:
	static StateSubject* m_stateSubject;

public:
	VnEngineSceneEditor(StateSubject* stateSubject) 
	{ 
		m_stateSubject = stateSubject;
	}

	~VnEngineSceneEditor() { }

private:
	// Encapsulate the combo header of this section
	class ComboActions {
	private:
		static int s_selectedComboIndex;
		static index s_selectedStep;

		static std::string addComboId(const char* str) { return (std::string(str) + std::string("##ComboChoiceId")); }

	public:
		static void draw() {
			ImGui::Spacing();
			ImGui::SeparatorText("Add Action");
			
			ImGui::Spacing();
			drawCombo();
			
			ImGui::Spacing();
			drawSelection();
			
			ImGui::Spacing();
			addActionToStep();
			
			ImGui::Spacing();
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
			Node* node{ static_cast<Node*>(ModelEngineInterface::getNodeById(m_stateSubject->getNodeId())) };
			
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
		static void draw() {
			if (m_stateSubject == nullptr) { return; }

			id nodeId{ m_stateSubject->getNodeId() };
			Node* nodeBase{ ModelEngineInterface::getNodeById(nodeId) };
			if (nodeBase == nullptr) { return; }

			// TODO: node objects to be brought into one object OR
			// do dynamic cast and handle failure of the cast?
			Node* node{ static_cast<Node*>(nodeBase) };

			ImGui::Spacing();
			ImGui::SeparatorText(node->getName().c_str());
			ImGui::Spacing();
			
			for (index i{ 0 }; i < node->getTotalSteps(); i++) {
				drawStep(node, i);
			}
		}


		static void drawStep(Node* node, index stepIndex) {
			if (node == nullptr) { return; }

			std::string stepTitle{ "Step #" + std::to_string(stepIndex) };

			bool isTreeOpen = ImGui::TreeNodeEx(stepTitle.c_str(), ImGuiTreeNodeFlags_Framed);

			// Dragging an entire step
			if (ImGui::BeginDragDropSource())
			{
				// Set payload to carry the index of our item (could be anything)
				ActionDragDropPayload payload{ 0, node->getId(), stepIndex };

				ImGui::SetDragDropPayload("ENTIRE_STEP", &payload, sizeof(ActionDragDropPayload));

				ImGui::Text("Dragging %s", stepTitle.c_str());

				ImGui::EndDragDropSource();
			}

			// Moving, copying, and swapping actions between steps
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ACTION_DRAG"))
				{
					handleDragDropPayload(payload, stepIndex, false);
				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTIRE_STEP"))
				{
					handleDragDropPayload(payload, stepIndex, true);
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
		
		static void handleDragDropPayload(const ImGuiPayload* payload, index stepIndex, bool isEntireStep) {
			assert(payload->DataSize == sizeof(ActionDragDropPayload));

			bool reloadState{ false };
			ActionDragDropPayload payloadCast = *(const ActionDragDropPayload*)payload->Data;
			payloadCast.m_destinationStepIndex = stepIndex;

			if (NodeEditorOptions::getMode() == ActionDragMode::DRAG_MOVE)
			{
				if (isEntireStep) {
					for (auto& item : s_items) {
						reloadState |= item.performMove(payloadCast);
					}
				}
				else {
					reloadState = s_items.at(payloadCast.m_typeIndex).performMove(payloadCast);
				}

				if (!reloadState) {
					EngineToolTip::setTooltipFor(500, "Nothing to move here!");
				}
				else {
					EngineToolTip::setTooltipFor(500, "Moved!");
				}
			}
			if (NodeEditorOptions::getMode() == ActionDragMode::DRAG_COPY)
			{
				if (isEntireStep) {
					for (auto& item : s_items) {
						reloadState |= item.performCopy(payloadCast);
					}
				}
				else {
					reloadState = s_items.at(payloadCast.m_typeIndex).performCopy(payloadCast);
				}
				
				if (!reloadState) {
					EngineToolTip::setTooltipFor(500, "Nothing to copy here!");
				}
				else {
					EngineToolTip::setTooltipFor(500, "Copied!");
				}

			}
			if (NodeEditorOptions::getMode() == ActionDragMode::DRAG_SWAP)
			{
				if (isEntireStep) {
					for (auto& item : s_items) {
						reloadState |= item.forceSwap(payloadCast);
					}
				}
				else {
					reloadState = s_items.at(payloadCast.m_typeIndex).performSwap(payloadCast);
				}
				
				if (!reloadState) {
					EngineToolTip::setTooltipFor(500, "Nothing to swap here!");
				}
				else {
					EngineToolTip::setTooltipFor(500, "Swapped!");
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
			ImGui::SeparatorText("Node Editor Options");

			// Dragging radio button options
			ImGui::Text("Action Dragging Mode: ");

			ImGui::SameLine();

			if (ImGui::RadioButton("Move", m_mode == ActionDragMode::DRAG_MOVE)) { m_mode = ActionDragMode::DRAG_MOVE; } 
			
			ImGui::SameLine();
			
			if (ImGui::RadioButton("Copy", m_mode == ActionDragMode::DRAG_COPY)) { m_mode = ActionDragMode::DRAG_COPY; } 
			
			ImGui::SameLine();
			
			if (ImGui::RadioButton("Swap", m_mode == ActionDragMode::DRAG_SWAP)) { m_mode = ActionDragMode::DRAG_SWAP; }
			

			// Add and delete step buttons
			if (ImGui::Button("Add Step", ImVec2(200, 50))) {
				Node* node{ static_cast<Node*>(ModelEngineInterface::getNodeById(m_stateSubject->getNodeId())) };

				if (node != nullptr) {
					NodeBuilder{ node }.incrementSteps();
				}
			}

			ImGui::SameLine();

			bool deleteButton = ImGui::Button("Delete", ImVec2(75, 50));
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
						EngineToolTip::setTooltipFor(500, "Deleted!");
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTIRE_STEP"))
				{
					assert(payload->DataSize == sizeof(ActionDragDropPayload));

					Node* node{ static_cast<Node*>(ModelEngineInterface::getNodeById(m_stateSubject->getNodeId())) };
					
					if (node != nullptr && node->getTotalSteps() > 1) {
						ActionDragDropPayload payloadCast = *(const ActionDragDropPayload*) payload->Data;

						for (auto& item : s_items) {
							item.performDelete(payloadCast);
						}

						for (index i{ payloadCast.m_sourceStepIndex + 1 }; i < node->getTotalSteps(); i++) {
							for (auto& item : s_items) {
								item.performMove({0, payloadCast.m_nodeId, i, i - 1});
							}
						}

						NodeBuilder{ node }.decrementSteps();
	
						m_stateSubject->goToNodeId(payloadCast.m_nodeId);
						if (m_stateSubject->getStepIndex() == node->getTotalSteps()) {
							m_stateSubject->goToStepIndex(node->getTotalSteps() - 1);
						}
						else {
							m_stateSubject->goToStepIndex(m_stateSubject->getStepIndex());
						}

						EngineToolTip::setTooltipFor(500, "Deleted!");
					}
					else {
						EngineToolTip::setTooltipFor(500, "Can't delete 0th step!");
					}

				}

				ImGui::EndDragDropTarget();
			}

			if (deleteButton) {
				EngineToolTip::setTooltipFor(500, "Drag steps/actions here to delete!");
			}

			// TODO: undo buttons?
		}
	};

	class NodeProperties {
	private:
		inline static id nodeIdToAdd{ 0 };

		static bool nodeExists(id nodeId) {
			return ModelCommonInterface::getNodeById(nodeId) != nullptr;
		}

		static bool drawLinkedNodesGrouping(Node* node, std::set<id>& list, bool isParentOf) {
			bool modified{ false };

			bool isTreeOpen{ false };

			if (isParentOf) {
				isTreeOpen = ImGui::TreeNodeEx(addIdFromPtr("Node Children###stayopen", &list).c_str(), ImGuiTreeNodeFlags_Framed);
			}
			else {
				isTreeOpen = ImGui::TreeNodeEx(addIdFromPtr("Node Parents###stayopen", &list).c_str(), ImGuiTreeNodeFlags_Framed);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ADD_TO_SET"))
				{
					assert(payload->DataSize == sizeof(id));
					id payloadCast = *(const id*)payload->Data;

					if (payloadCast > 0 && nodeExists(payloadCast)) {
						if (isParentOf) {
							NodeBuilder{ node }.link(ModelCommonInterface::getNodeById(payloadCast));
						}
						else {
							NodeBuilder{ ModelCommonInterface::getNodeById(payloadCast) }.link(node);
						}
					}
					else {
						EngineToolTip::setTooltipFor(500, "Invalid Node Id!");
					}
				}
				
				modified = true;

				ImGui::EndDragDropTarget();
			}

			if (isTreeOpen) {
				int i{ 0 };
				for (auto iter{ list.begin() }; iter != list.end(); iter++) {
					ImGui::Text((std::string("#") + std::to_string(i)).c_str());
					ImGui::SameLine();
					ImGui::PushItemWidth(100.0f);

					id dummyInt{ *iter };
					ImGui::DragInt(addIdFromPtr("Value", &(*iter)).c_str(), &dummyInt, 0.0f, *iter, *iter, "%d", 0);

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						std::string idAsStr{ std::to_string(*iter) };

						if (isParentOf) {
							ImGui::SetDragDropPayload("DELETE_CHILD", &(*iter), sizeof(id));
							ImGui::Text((std::string("child node ") + idAsStr).c_str());
						}
						else {
							ImGui::SetDragDropPayload("DELETE_PARENT", &(*iter), sizeof(id));
							ImGui::Text((std::string("parent node ") + idAsStr).c_str());
						}

						ImGui::EndDragDropSource();
					}

					ImGui::PopItemWidth();

					i++;
				}



				ImGui::TreePop();
			}
		
			return modified;
		}


	public:
		static void draw() {
			ImGui::Spacing();
			ImGui::SeparatorText("Node Properties");

			if (m_stateSubject == nullptr) { return; }

			id nodeId{ m_stateSubject->getNodeId() };
			Node* node{ ModelEngineInterface::getNodeById(nodeId) };
			if (node == nullptr) { return; }

			// Display node id
			ImGui::Text("Node Id: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(nodeId).c_str());
			
			// Edit node name
			static ImGuiInputTextFlags textFlags{ 0 };
			std::string nodeName = NodeBuilder{ node }.getName();
			ImGui::PushItemWidth(150.0f);
			ImGui::Text("Node Name:");
			ImGui::SameLine();
			bool nodeNameModified{ ImGui::InputText(addIdFromPtr("##nodeName", &(node->m_name)).c_str(), &(nodeName), textFlags) };
			ImGui::PopItemWidth();

			if (nodeNameModified) { NodeBuilder{ node }.setName(nodeName); }
			
			bool isTreeOpen = ImGui::TreeNodeEx("Parents and Children###stayopen", ImGuiTreeNodeFlags_Framed);

			if (isTreeOpen) {
				// Add/remove node parents
				bool parentsModified = drawLinkedNodesGrouping(node, LinkableBuilder{ node }.getParents(), false);

				ImGui::Spacing();

				// Add/remove node children
				bool childrenModified = drawLinkedNodesGrouping(node, LinkableBuilder{ node }.getChildren(), true);

				// Delete button for parents or children
				bool deleteClicked = ImGui::Button("Delete##DeleteAParentOrChildNodeLink", ImVec2(260.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DELETE_PARENT"))
					{
						assert(payload->DataSize == sizeof(id));
						id payloadCast = *(const id*)payload->Data;

						NodeBuilder{ ModelCommonInterface::getNodeById(payloadCast) }.unlink(node);
					}
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DELETE_CHILD"))
					{
						assert(payload->DataSize == sizeof(id));
						id payloadCast = *(const id*)payload->Data;

						NodeBuilder{ node }.unlink(ModelCommonInterface::getNodeById(payloadCast));
					}

					ImGui::EndDragDropTarget();
				}

				if (deleteClicked) {
					EngineToolTip::setTooltipFor(500, "Drag parent/child here to delete a link!");
				}

				// Add a new node as parent or child
				ImGui::Text("Drag To Add Node Id: ");
				ImGui::SameLine();

				ImGui::PushItemWidth(129.0f);
				ImGui::DragInt(addIdFromPtr("####", &(nodeIdToAdd)).c_str(), &nodeIdToAdd, 0.0f, nodeIdToAdd, nodeIdToAdd, "%d", 0);
				ImGui::PopItemWidth();
				
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("ADD_TO_SET", &(nodeIdToAdd), sizeof(id));
					ImGui::Text((std::string("Adding ") + std::to_string(nodeIdToAdd)).c_str());

					ImGui::EndDragDropSource();
				}

				ImGui::TreePop();
			}
		}
	};


private:
	// TODO: is this needed?
	void updateViewport() {

	}

public:
	void draw() {		
		NodeProperties::draw();

		ComboActions::draw();

		NodeSteps::draw();

		NodeEditorOptions::draw();
	}
};


#endif // VN_ENGINE_NODE_EDITOR_SIDEBAR_H