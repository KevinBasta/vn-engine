#ifndef VN_ENGINE_GRAPH_H
#define VN_ENGINE_GRAPH_H

#include "id.h"
#include "window.h"
#include "context.h"

#include "model_subject.h"
#include "engine_chapter_manager.h"

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

namespace ed = ax::NodeEditor;

namespace std
{
	template<> struct hash<ed::PinId>
	{
		std::size_t operator()(const ed::PinId& s) const noexcept
		{
			return static_cast<std::size_t>(s.Get());
		}
	};
	
	template<> struct hash<ed::NodeId>
	{
		std::size_t operator()(const ed::NodeId& s) const noexcept
		{
			return static_cast<std::size_t>(s.Get());
		}
	};
}

class VnEngineGraph {
public:
	struct PinIdHasher {
		size_t operator()(const ed::PinId& key) const {
			return std::hash<ed::PinId>()(key);
		}
	};
	
	struct NodeIdHasher {
		size_t operator()(const ed::NodeId& key) const {
			return std::hash<ed::NodeId>()(key);
		}
	};
	
	struct NodeLinkKey;

	struct NodeLinkKeyHasher {
		size_t operator()(const NodeLinkKey& key) const {
			return std::hash<id>()(key.m_firstNode) ^
				(std::hash<id>()(key.m_secondNode) << 1);
		}
	};

	struct NodeLinkKey {
		id m_firstNode{};
		id m_secondNode{};

		friend bool operator==(const NodeLinkKey& keyOne, const NodeLinkKey& keyTwo) {
			NodeLinkKeyHasher hasher{};
			return hasher(keyOne) == hasher(keyTwo);
		}
	};

	struct NodeLinkData {
		ed::LinkId m_id{};
		ed::PinId m_inId{};
		ed::PinId m_outId{};
	};

protected:
	ed::EditorContext* m_context = nullptr;
	bool m_firstFrame = true;
	int m_uniqueId{ 1 };
	int m_linkId{ 1 };

	std::unordered_map<NodeLinkKey, NodeLinkData, NodeLinkKeyHasher> m_currentLinks{};
	std::unordered_map<ed::PinId, id, PinIdHasher> m_pinInIdToNodeId{};
	std::unordered_map<ed::PinId, id, PinIdHasher> m_pinOutIdToNodeId{};
	std::set<id> m_drawnNodes{};
	std::set<id> m_graphNodes{};
	std::set<id> m_strayNodes{};

public:
	VnEngineGraph() {
		ed::Config config;
		config.SettingsFile = "BasicInteraction.json";
		m_context = ed::CreateEditor(&config);
	}

	~VnEngineGraph() {
		ed::DestroyEditor(m_context);
	}

	void draw() {
		auto& io = ImGui::GetIO();
		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
		ImGui::Separator();

		ed::SetCurrentEditor(m_context);
		ed::Begin("My Editor", ImVec2(0.0, 0.0f));

		m_uniqueId = IdGenerator<Linkable>::returnGreatestId() + 1;
		m_linkId = 1;
		m_currentLinks.clear();
		m_pinInIdToNodeId.clear();
		m_pinOutIdToNodeId.clear();
		m_drawnNodes.clear();
		m_graphNodes.clear();

		int x = 10;
		int y = 10;

		// Draw the chapter graph/tree
		const Chapter* headChapter{ ModelSubject::getHeadChapter() };
		drawChapter(headChapter, x, y);
		m_graphNodes = m_drawnNodes;

		// Draw the nodes that have been disconnected
		for (auto& chapterId : m_strayNodes) {
			Chapter* chapter{ ModelSubject::getChapterById(chapterId) };

			if (chapter != nullptr && !m_drawnNodes.contains(chapterId)) {
				drawChapter(chapter, 0, 0);
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
						
						// TODO: Error checking
						id parentId{ (m_pinOutIdToNodeId[outputPinId] == 0) ? m_pinOutIdToNodeId[inputPinId] : m_pinOutIdToNodeId[outputPinId] };
						id childId{ (m_pinInIdToNodeId[inputPinId] == 0) ? m_pinInIdToNodeId[outputPinId] : m_pinInIdToNodeId[inputPinId] };

						Chapter* parentNode{ ModelSubject::getChapterById(parentId) };
						Chapter* childNode{ ModelSubject::getChapterById(childId) };

						if (parentNode != nullptr && childNode != nullptr) {
							ChapterBuilder{ parentNode }.link(childNode);


							// Only erase a node from the stray nodes if it's not linking to itself and is a stray node
							// TODO: handle loop with no other parent
							if (parentId != childId) {
								bool isStrayNode{ m_strayNodes.find(childId) != m_strayNodes.end() };
								bool isJoinedToGraph{ m_graphNodes.contains(parentNode->getId()) };

								if (isStrayNode && isJoinedToGraph) {
									m_strayNodes.erase(childId);
								}
							}
						}

						// BUG: draging one node drags a different pin id, need to make the node id conform to unique id // DONE
						// BUG: head node in can't be dragged from // DONE
						// BUG: two stray nodes forming a loop disappear // DONE
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
							std::cout << m_pinInIdToNodeId[data.m_inId] << std::endl;
							std::cout << m_pinOutIdToNodeId[data.m_outId] << std::endl;

							id parentId{ m_pinOutIdToNodeId[data.m_outId] };
							id childId{ m_pinInIdToNodeId[data.m_inId] };

							Chapter* parentNode{ ModelSubject::getChapterById(parentId) };
							Chapter* childNode{ ModelSubject::getChapterById(childId) };

							if (parentNode == nullptr) {
								std::cout << "parent is null" << std::endl;
							}

							if (childNode == nullptr) {
								std::cout << "child is null" << std::endl;
							}

							ChapterBuilder{ parentNode }.unlink(childNode);

							//ed::GetNodeZPosition(childNode->getId());
							//ed::NodeId(childNode->getId());

							if (childNode != nullptr && childNode->getParentsAmount() == 0) {
								m_strayNodes.insert(childNode->getId());
							}

							//ed::DeleteLink(deletedLinkId);

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

		if (m_firstFrame) {
			ed::NavigateToContent(0.0f);
		}

		ed::SetCurrentEditor(nullptr);
		m_firstFrame = false;
	}

	void drawChapter(const Chapter* chapter, int x, int y) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
		std::string convertedName{ myconv.to_bytes(chapter->getName()) };

		ed::NodeId nodeId = chapter->getId();
		ed::PinId  inPinId = m_uniqueId++;
		ed::PinId  outPinId = m_uniqueId++;

		m_drawnNodes.insert(chapter->getId());
		m_pinInIdToNodeId[inPinId] = chapter->getId();
		m_pinOutIdToNodeId[outPinId] = chapter->getId();
		
		// Draw the node in the graph window
		if (m_firstFrame)
			ed::SetNodePosition(nodeId, ImVec2(x, y));

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

		// Populate graph link information for both parent and child nodes
		for (auto chapterId : chapter->getParentsSet()) {
			if (m_currentLinks.find({ chapterId, chapter->getId() }) == m_currentLinks.end()) {
				m_currentLinks[{ chapterId, chapter->getId() }].m_id = m_linkId++;
			}

			m_currentLinks[{ chapterId, chapter->getId() }].m_inId = inPinId;
		}

		for (auto chapterId : chapter->getChildrenSet()) {
			if (m_currentLinks.find({ chapter->getId(), chapterId }) == m_currentLinks.end()) {
				m_currentLinks[{chapter->getId(), chapterId}].m_id = m_linkId++;
			}

			m_currentLinks[{chapter->getId(), chapterId}].m_outId = outPinId;
		}

		// Recursion for drawing child nodes
		std::list<const Chapter*> nextChapters{};
		for (auto chapterId : chapter->getChildrenSet()) {
			//std::cout << "chapterId" << chapterId << std::endl;
			const Chapter* childChapter{ ModelSubject::getChapterById(chapterId) };

			if (childChapter != nullptr && chapterId != chapter->getId() && !m_drawnNodes.contains(chapterId)) {
				nextChapters.push_back(childChapter);
			}
		}

		int nextXStart{ x + 170 };
		int nextYStart{ y - ((static_cast<int>(nextChapters.size()) * 100) / 2) };

		for (auto childChapter : nextChapters) {
			drawChapter(childChapter, nextXStart, nextYStart);
			nextYStart += 100;
		}

	}

};

#endif // VN_ENGINE_GRAPH_H