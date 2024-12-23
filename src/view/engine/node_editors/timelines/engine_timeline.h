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
	int m_operationalId{ 1 }; // Unsure if linkid needs to be included or can be separate. It's included in this.
	
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

public:
	//
	// Rendering functions
	//
	void draw() {
		// 1) Drawing setup
		auto& io = ImGui::GetIO();

		// TODO: remove FPS?
		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
		ImGui::Separator();

		ed::SetCurrentEditor(m_context);
		ed::Begin(getGraphName().c_str(), ImVec2(0.0, 0.0f));

		// Reset the timeline nodes
		m_operationalId = getTimlineRangeMax() + 1;
		m_currentLinks.clear();
		m_nodeIdToLinkableId.clear();
		m_pinInIdToNodeId.clear();
		m_pinOutIdToNodeId.clear();

		// Initial timeline node position
		int x = 10;
		int y = 10;

		// Draw the timeline
		drawTimelineNodes(x, y);
		
		// Draw all links
		for (auto& [key, data] : m_currentLinks) {
			ed::Link(data.m_id, data.m_inId, data.m_outId);
		}


		// 2) Handle interactions

		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ed::BeginCreate()) {

			// QueryNewLink returns true if editor want to create new link between pins.
			ed::PinId inputPinId, outputPinId;
			if (ed::QueryNewLink(&inputPinId, &outputPinId)) {
				// both are valid, let's accept link
				if (inputPinId && outputPinId) {
					// ed::AcceptNewItem() return true when user release mouse button.
					/*if (ed::AcceptNewItem()) {
						id parentId{ (m_pinOutIdToNodeId[outputPinId] == 0) ? m_pinOutIdToNodeId[inputPinId] : m_pinOutIdToNodeId[outputPinId] };
						id childId{ (m_pinInIdToNodeId[inputPinId] == 0) ? m_pinInIdToNodeId[outputPinId] : m_pinInIdToNodeId[inputPinId] };

						Linkable* parentLinkable{ getLinkableById(parentId) };
						Linkable* childLinkable{ getLinkableById(childId) };

						if (parentLinkable != nullptr && childLinkable != nullptr) {
							LinkableBuilder{ parentLinkable }.link(childLinkable);

							// Only erase a node from the stray nodes if it's not linking to itself
							// Add it to pending stray nodes in case of stray loops							
							// TODO: This can be compeltely removed as it's handled by the update logic
							if (parentId != childId) {
								bool isChildAStrayNode{ m_strayNodes[getLinkableHeadId()].find(childId) != m_strayNodes[getLinkableHeadId()].end() };

								if (isChildAStrayNode) {
									m_strayNodes[getLinkableHeadId()].erase(childId);
									m_pendingStrayNodes.insert(childId);
								}
							}
						}
					}*/

					ed::RejectNewItem();

					// You may choose to reject connection between these nodes
					// by calling ed::RejectNewItem(). This will allow editor to give
					// visual feedback by changing link thickness and color.
				}
			}
		}
		ed::EndCreate(); // Wraps up object creation action handling.


		// Handle deletion action
		if (ed::BeginDelete())
		{
			// There may be many links marked for deletion, let's loop over them.
			ed::LinkId deletedLinkId;
			while (ed::QueryDeletedLink(&deletedLinkId))
			{
				// If you agree that link can be deleted, accept deletion.
				/*if (ed::AcceptDeletedItem())
				{
					// Then remove link from your data.
					for (auto& [key, data] : m_currentLinks)
					{
						if (data.m_id == deletedLinkId)
						{
							id parentId{ m_pinOutIdToNodeId[data.m_outId] };
							id childId{ m_pinInIdToNodeId[data.m_inId] };

							Linkable* parentLinkable{ getLinkableById(parentId) };
							Linkable* childLinkable{ getLinkableById(childId) };

							LinkableBuilder{ parentLinkable }.unlink(childLinkable);

							// Set child to be set as a stray node after checking next head graph render
							m_pendingStrayNodes.insert(childId);

							break;
						}
					}
				}*/

				// You may reject link deletion by calling:
				ed::RejectDeletedItem();
			}
		}
		ed::EndDelete();
		ed::End();


		ed::NodeId doubleClickedNode{ ed::GetDoubleClickedNode() };

		if (doubleClickedNode && !isCurrentlySelected(m_nodeIdToLinkableId[doubleClickedNode])) {
			setCurrentlySelectedToIndex(m_nodeIdToLinkableId[doubleClickedNode]);
		}
		else {
			//ed::SelectNode(currentStateLinkable, true);
		}

		if (m_doInitialNavigation) {
			// The graphs are not drawn in the first gameloop
			// so navigate to the graph content at the 5th gameloop
			if (m_navigateToContent == 5) {
				std::cout << "navigate to content" << std::endl;
				ed::NavigateToContent(0.0f);
				m_doInitialNavigation = false;
			}
			else {
				m_navigateToContent++;
			}
		}

		// If the head node changes, navigate to the new graph
		/*if (m_previousHead != getLinkableHeadId()) {
			ed::NavigateToContent(3.0f);
			m_previousHead = getLinkableHeadId();
		}*/

		ed::SetCurrentEditor(nullptr);
		m_firstFrame = false;
	}

	void drawTimelineNodes(int x = 0, int y = 0) {
		std::pair<index, index> range{ getTimelineRange() };

		for (index i{ range.first }; i <= range.second; i++) {
			std::string name{ std::to_string(i) };

			// Populate node and pin data
			ed::NodeId nodeId = i;
			ed::PinId  inPinId = m_operationalId++;
			ed::PinId  outPinId = m_operationalId++;

			m_nodeIdToLinkableId[nodeId] = i;
			m_pinInIdToNodeId[inPinId] = i;
			m_pinOutIdToNodeId[outPinId] = i;

			// Handle special cases
			if (m_firstFrame) {
				ed::SetNodePosition(nodeId, ImVec2(x, y));
				x += 100;
			}

			if (isCurrentlySelected(i)) {
				ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
			}

			// Draw the node in the graph window
			ed::BeginNode(nodeId);

			ImGui::Text(name.c_str());

			ImGui::BeginGroup();
			ed::BeginPin(inPinId, ed::PinKind::Input);
			ImGui::Text("-> In");
			ed::EndPin();
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			ed::BeginPin(outPinId, ed::PinKind::Output);
			ImGui::Text("Out ->");
			ed::EndPin();
			ImGui::EndGroup();

			ed::EndNode();

			if (isCurrentlySelected(i)) {
				ed::PopStyleColor(1);
			}

			// Populate link data
			// No links needed if there's only one item in range
			if (range.first != range.second) {
				if (i == range.first) {
					// If currently on the first link then populate link to next
					if (m_currentLinks.find({ i, i + 1 }) == m_currentLinks.end()) {
						m_currentLinks[{i, i + 1}].m_id = m_operationalId++;
					}

					m_currentLinks[{i, i + 1}].m_outId = outPinId;
				}
				else if (i == range.second) {
					// If currently on the last link then populate link to previous
					if (m_currentLinks.find({ i - 1, i }) == m_currentLinks.end()) {
						m_currentLinks[{i - 1, i}].m_id = m_operationalId++;
					}

					m_currentLinks[{i - 1, i}].m_inId = inPinId;
				}
				else {
					// If in the middle of the range then populate previous and next links
					if (m_currentLinks.find({ i - 1, i }) == m_currentLinks.end()) {
						m_currentLinks[{i - 1, i}].m_id = m_operationalId++;
					}

					m_currentLinks[{i - 1, i}].m_inId = inPinId;
					
					if (m_currentLinks.find({ i, i + 1 }) == m_currentLinks.end()) {
						m_currentLinks[{i, i + 1}].m_id = m_operationalId++;
					}

					m_currentLinks[{i, i + 1}].m_outId = outPinId;
				}
			}
		}
	}


};

#endif // VN_ENGINE_TIMELINE_H