#ifndef VN_ENGINE_GRAPH_H
#define VN_ENGINE_GRAPH_H

#include "id.h"
#include "window.h"
#include "context.h"

#include "model_subject.h"
#include "engine_chapter_manager.h"
#include "graph_editors_common.h"

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

class VnEngineGraph {
protected:
	ed::EditorContext* m_context{ nullptr };
	bool m_firstFrame{ true };
	
	bool m_doInitialNavigation{ true };
	int m_navigateToContent{ 0 };

	int m_uniqueId{ 1 };
	int m_linkId{ 1 };
	id m_previousHead{ 0 };
	// TODO: update selected graph node based on state subject
	// when it switches through preview interaction, switch here
	// when you switch here, change the state subject iterator

	std::unordered_map<NodeLinkKey, NodeLinkData, NodeLinkKeyHasher> m_currentLinks{};
	std::unordered_map<ed::NodeId, id, NodeIdHasher> m_nodeIdToLinkableId{};
	std::unordered_map<ed::PinId, id, PinIdHasher> m_pinInIdToNodeId{};
	std::unordered_map<ed::PinId, id, PinIdHasher> m_pinOutIdToNodeId{};
	std::set<id> m_drawnNodes{};
	std::set<id> m_graphNodes{};
	std::unordered_map<id, std::set<id>> m_strayNodes{};
	std::set<id> m_pendingStrayNodes{};

public:
	VnEngineGraph(const char* filename) {
		ed::Config config;
		config.SettingsFile = filename;
		m_context = ed::CreateEditor(&config);
		// TODO: deselect everything on first frame? May need to change imgui node editor config file
		// TODO: serialize the current chapter and node (and step?) editing for project files?
	}

	virtual ~VnEngineGraph() {
		ed::DestroyEditor(m_context);
	}

	virtual std::string getGraphName() = 0;
	virtual id getLinkableHeadId() = 0;
	virtual const Linkable* getLinkableHead() = 0;
	virtual Linkable* getLinkableById(id linkableId) = 0;
	virtual std::wstring getLinkableName(id linkableId) = 0;
	virtual id getCurrentStateLinkable() = 0;
	virtual void setCurrentStateToLinkable(id linkableId) = 0;

	void draw() {
		auto& io = ImGui::GetIO();
		
		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
		ImGui::Separator();

		ed::SetCurrentEditor(m_context);
		ed::Begin(getGraphName().c_str(), ImVec2(0.0, 0.0f));

		m_uniqueId = IdGenerator<Linkable>::returnGreatestId() + 1;
		m_linkId = 1;
		m_currentLinks.clear();
		m_pinInIdToNodeId.clear();
		m_pinOutIdToNodeId.clear();
		m_drawnNodes.clear();
		m_graphNodes.clear();
		m_strayNodes.clear();
		m_pendingStrayNodes.clear();
		m_nodeIdToLinkableId.clear();

		int x = 10;
		int y = 10;

		// Draw the chapter graph/tree
		const Linkable* headLinkable{ getLinkableHead() };
		drawGraphNode(headLinkable, x, y);
		m_graphNodes = m_drawnNodes;

		// Check if pending stray nodes are not connected to the head graph
		for (id pendingStray : m_pendingStrayNodes) {
			bool isStray{ true };

			Linkable* currentLinkable{ getLinkableById(pendingStray) };

			if (currentLinkable == nullptr) {
				continue;
			}

			for (auto parentId : currentLinkable->getParents()) {
				if (m_graphNodes.contains(parentId)) {
					isStray = false;
					break;
				}
			}

			if (isStray) {
				m_strayNodes[getLinkableHeadId()].insert(pendingStray);
			}
		}

		m_pendingStrayNodes.clear();

		// Draw the nodes that have been marked as stray, removing any from the list if they have been drawn already
		for (std::set<id>::iterator iter{ m_strayNodes[getLinkableHeadId()].begin() }; iter != m_strayNodes[getLinkableHeadId()].end(); iter++) {
			Linkable* linkable{ getLinkableById(*iter) };

			if (linkable != nullptr) {
				if (!m_drawnNodes.contains(*iter)) {
					drawGraphNode(linkable);
				}
				else {
					iter = m_strayNodes[getLinkableHeadId()].erase(iter);
					
					if (iter == m_strayNodes[getLinkableHeadId()].end()) {
						break;
					}
				}
			}
		}
		
		// Draw all links
		for (auto& [key, data] : m_currentLinks) {
			ed::Link(data.m_id, data.m_inId, data.m_outId);
		}


		//
		// 2) Handle interactions
		//

		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ed::BeginCreate()) {
			
			// QueryNewLink returns true if editor want to create new link between pins.
			ed::PinId inputPinId, outputPinId;
			if (ed::QueryNewLink(&inputPinId, &outputPinId)) {
				// both are valid, let's accept link
				if (inputPinId && outputPinId) {
					// ed::AcceptNewItem() return true when user release mouse button.
					if (ed::AcceptNewItem()) {						
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
					}

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
				if (ed::AcceptDeletedItem())
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
							//m_pendingStrayNodes.insert(childId);

							break;
						}
					}
				}

				// You may reject link deletion by calling:
				// ed::RejectDeletedItem();
			}
		}
		ed::EndDelete();
		ed::End();


		ed::NodeId doubleClickedNode{ ed::GetDoubleClickedNode() };
		
		if (doubleClickedNode && m_nodeIdToLinkableId[doubleClickedNode] != getCurrentStateLinkable()) {
			setCurrentStateToLinkable(m_nodeIdToLinkableId[doubleClickedNode]);
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
		if (m_previousHead != getLinkableHeadId()) {
			ed::NavigateToContent(3.0f);
			m_previousHead = getLinkableHeadId();
		}

		ed::SetCurrentEditor(nullptr);
		m_firstFrame = false;
	}

	void drawGraphNode(const Linkable* linkable, int x = 0, int y = 0) {
		if (linkable == nullptr) { return; }

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
		std::string convertedName{ myconv.to_bytes(getLinkableName(linkable->getId())) };

		id linkableId{ linkable->getId() };

		ed::NodeId nodeId = linkableId;
		ed::PinId  inPinId = m_uniqueId++;
		ed::PinId  outPinId = m_uniqueId++;

		m_drawnNodes.insert(linkableId);
		m_nodeIdToLinkableId[nodeId] = linkableId;
		m_pinInIdToNodeId[inPinId] = linkableId;
		m_pinOutIdToNodeId[outPinId] = linkableId;
		
		// Draw the node in the graph window
		if (m_firstFrame)
			ed::SetNodePosition(nodeId, ImVec2(x, y));

		if (getCurrentStateLinkable() == linkableId) {
			ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
		}

		ed::BeginNode(nodeId);

		ImGui::Text(convertedName.c_str());

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

		if (getCurrentStateLinkable() == linkableId) {
			ed::PopStyleColor(1);
		}
		
		// Populate graph link information for both parent and child nodes
		populateParentLinks(linkable, inPinId);
		populateChildLinks(linkable, outPinId);

		// Recursion for drawing child nodes
		drawGraphChildNodes(linkable, x, y);
	}

	void populateParentLinks(const Linkable* linkable, ed::PinId inPinId) {
		for (auto connectedLinkableId : linkable->getParents()) {
			if (m_currentLinks.find({ connectedLinkableId, linkable->getId() }) == m_currentLinks.end()) {
				m_currentLinks[{ connectedLinkableId, linkable->getId() }].m_id = m_linkId++;
			}

			m_currentLinks[{ connectedLinkableId, linkable->getId() }].m_inId = inPinId;
		}
	}

	void populateChildLinks(const Linkable* linkable, ed::PinId outPinId) {
		for (auto connectedLinkableId : linkable->getChildren()) {
			if (m_currentLinks.find({ linkable->getId(), connectedLinkableId }) == m_currentLinks.end()) {
				m_currentLinks[{linkable->getId(), connectedLinkableId}].m_id = m_linkId++;
			}

			m_currentLinks[{linkable->getId(), connectedLinkableId}].m_outId = outPinId;
		}
	}

	void drawGraphChildNodes(const Linkable* linkable, int x = 0, int y = 0) {
		// Create a list of linkable objects who's pointers are obtainable
		std::list<const Linkable*> validChildLinkables{};
		for (auto connectedLinkableId : linkable->getChildren()) {
			const Linkable* childLinkable{ getLinkableById(connectedLinkableId) };

			if (childLinkable != nullptr && connectedLinkableId != linkable->getId() && !m_drawnNodes.contains(connectedLinkableId)) {
				validChildLinkables.push_back(childLinkable);
			}
		}

		// Recursion to draw all the child graph nodes
		int nextXStart{ x + 170 };
		int nextYStart{ y - ((static_cast<int>(validChildLinkables.size()) * 100) / 2) };

		for (auto childLinkable : validChildLinkables) {
			drawGraphNode(childLinkable, nextXStart, nextYStart);
			nextYStart += 100;
		}
	}
};

#endif // VN_ENGINE_GRAPH_H