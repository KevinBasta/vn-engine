#ifndef VN_ENGINE_TIMELINE_H
#define VN_ENGINE_TIMELINE_H

#include "id.h"
#include "index.h"
#include "window.h"
#include "context.h"

#include "model_subject.h"
#include "engine_chapter_manager.h"
#include "node_editors_common.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <codecvt>
#include <string>
#include <vector>
#include <unordered_map>

#include <GLFW/glfw3.h>

class VnEngineTimeline {
protected:
	//
	// Class variables
	//
	ed::EditorContext* m_context{ nullptr };
	
	// Operational Data of nodes and links
	std::unordered_map<NodeLinkKey, NodeLinkData, NodeLinkKeyHasher> m_currentLinks{};
	std::unordered_map<ed::NodeId, index, NodeIdHasher> m_nodeIdToLinkableId{};
	std::unordered_map<ed::PinId, index, PinIdHasher> m_pinInIdToNodeId{};
	std::unordered_map<ed::PinId, index, PinIdHasher> m_pinOutIdToNodeId{};

	// For giving the nodes and pins all different Ids which is required by the imgui node editor library
	// (otherwise two entities with the same id will have one clickable and one not clickable for example)
	int m_operationalId{ 1 };
	int m_linkId{ 1 };
	
	// For config the timeline on init
	bool m_firstFrame{ true };
	bool m_doInitialNavigation{ true };
	int m_navigateToContent{ 0 };

protected:
	//
	// Functions to be defined in the derived class
	// This class is meant to be generic to accomedate steps and animations
	// With potential for future extentions like a sound effects timeline
	//
	virtual std::string getGraphName() = 0;
	
	virtual std::pair<index, index> getTimelineRange() = 0; // [x, y] inclusive range
	virtual index getTimlineRangeMax() = 0;

	virtual bool isCurrentlySelected(index rangeIndex) = 0;
	virtual void setCurrentlySelectedToIndex(index rangeIndex) = 0;
	virtual bool handleDeletingNode(index rangeIndex) = 0;

	virtual bool shouldRedrawPositions() = 0;

	virtual ImColor getSelectionColor() = 0;

public:
	//
	// Constructor and destructor
	//
	VnEngineTimeline(const char* filename) {
		ed::Config config;
		config.SettingsFile = filename;
		m_context = ed::CreateEditor(&config);
		// TODO: deselect everything on first frame? May need to change imgui node editor config file
		// TODO: serialize the current chapter and node (and step?) editing for project files?
	}

	virtual ~VnEngineTimeline() {
		ed::DestroyEditor(m_context);
	}

	//
	// Rendering functions
	//
	void draw() {
		ed::SetCurrentEditor(m_context);
		ed::Begin(getGraphName().c_str(), ImVec2(0.0, 0.0f));

		resetTimeline();
		drawTimelineNodesAndLinks();
		handleInteractions();

		ed::End();

		if (m_doInitialNavigation) {
			// The graphs are not drawn in the first gameloop
			// so navigate to the graph content at the 5th gameloop
			if (m_navigateToContent == 5) {
				ed::NavigateToContent(0.0f);
				m_doInitialNavigation = false;
			}
			else {
				m_navigateToContent++;
			}
		}

		ed::SetCurrentEditor(nullptr);
		m_firstFrame = false;
	}



private:
	//
	// Private rendering functions
	//

	void resetTimeline() {
		// Reset the timeline nodes
		m_operationalId = getTimlineRangeMax() + 1;
		m_linkId = 1;
		m_currentLinks.clear();
		m_nodeIdToLinkableId.clear();
		m_pinInIdToNodeId.clear();
		m_pinOutIdToNodeId.clear();
	}


	void drawTimelineNodesAndLinks() {
		// Draw the nodes (and populate data)
		drawTimelineNodes(10, 10);

		// Draw the links
		for (auto& [key, data] : m_currentLinks) {
			ed::Link(data.m_id, data.m_inId, data.m_outId);
		}
	}

	void drawTimelineNodes(int x = 0, int y = 0) {
		std::pair<index, index> range{ getTimelineRange() };
		bool redraw{ shouldRedrawPositions() };

		for (index i{ range.first }; i <= range.second; i++) {
			std::string name{ std::to_string(i) };

			// Populate node and pin data

			// Making sure a nodeId is never 0 due to library issue with id 0
			ed::NodeId nodeId = i + 1; 
			ed::PinId  inPinId = m_operationalId++;
			ed::PinId  outPinId = m_operationalId++;

			m_nodeIdToLinkableId[nodeId] = i;
			m_pinInIdToNodeId[inPinId] = i;
			m_pinOutIdToNodeId[outPinId] = i;

			// Handle special cases
			if (redraw) {
				ed::SetNodePosition(nodeId, ImVec2(x, y));
				x += 100;
			}

			bool isSelected{ isCurrentlySelected(i) };

			if (isSelected) {
				ed::PushStyleColor(ed::StyleColor_NodeBg, getSelectionColor());
			}

			// Draw the node in the graph window
			ed::BeginNode(nodeId);

			ImGui::Text(name.c_str());

			ImGui::BeginGroup();
			ed::BeginPin(inPinId, ed::PinKind::Input);
			ImGui::Text(" ");
			ed::EndPin();
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			ed::BeginPin(outPinId, ed::PinKind::Output);
			ImGui::Text(" ");
			ed::EndPin();
			ImGui::EndGroup();

			ed::EndNode();

			if (isSelected) {
				ed::PopStyleColor(1);
			}

			// Populate link data
			// No links needed if there's only one item in range
			if (range.first != range.second) {
				if (i == range.first) {
					// If currently on the first link then populate link to next
					if (m_currentLinks.find({ i, i + 1 }) == m_currentLinks.end()) {
						m_currentLinks[{i, i + 1}].m_id = m_linkId++;
					}

					m_currentLinks[{i, i + 1}].m_outId = outPinId;
				}
				else if (i == range.second) {
					// If currently on the last link then populate link to previous
					if (m_currentLinks.find({ i - 1, i }) == m_currentLinks.end()) {
						m_currentLinks[{i - 1, i}].m_id = m_linkId++;
					}

					m_currentLinks[{i - 1, i}].m_inId = inPinId;
				}
				else {
					// If in the middle of the range then populate previous and next links
					if (m_currentLinks.find({ i - 1, i }) == m_currentLinks.end()) {
						m_currentLinks[{i - 1, i}].m_id = m_linkId++;
					}

					m_currentLinks[{i - 1, i}].m_inId = inPinId;
					
					if (m_currentLinks.find({ i, i + 1 }) == m_currentLinks.end()) {
						m_currentLinks[{i, i + 1}].m_id = m_linkId++;
					}

					m_currentLinks[{i, i + 1}].m_outId = outPinId;
				}
			}
		}

		if (redraw) {
			ed::NavigateToContent(0.0f);
		}

	}

	void handleInteractions() {
		// Handle link creation action
		if (ed::BeginCreate()) {
			ed::PinId inputPinId, outputPinId;
			if (ed::QueryNewLink(&inputPinId, &outputPinId)) {
				ed::RejectNewItem();
			}
		}
		ed::EndCreate();

		// Handle link deletion action
		if (ed::BeginDelete()) {
			ed::LinkId deletedLinkId;
			while (ed::QueryDeletedLink(&deletedLinkId))
			{
				ed::RejectDeletedItem();
			}
		}
		ed::EndDelete();

		// Handle node deletion action
		if (ed::BeginDelete()) {
			ed::NodeId deletedNodeId;
			while (ed::QueryDeletedNode(&deletedNodeId)) {
				if (handleDeletingNode(m_nodeIdToLinkableId[deletedNodeId])) {
					ed::AcceptDeletedItem();
				}
				else {
					ed::RejectDeletedItem();
				}
			}
		}
		ed::EndDelete();
	
		// Handle double clicking node
		ed::NodeId doubleClickedNode{ ed::GetDoubleClickedNode() };

		if (doubleClickedNode && !isCurrentlySelected(m_nodeIdToLinkableId[doubleClickedNode])) {
			setCurrentlySelectedToIndex(m_nodeIdToLinkableId[doubleClickedNode]);
		}
	}


};

#endif // VN_ENGINE_TIMELINE_H