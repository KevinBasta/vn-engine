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
}

class VnEngineGraph {
public:
	struct PinIdHasher {
		size_t operator()(const ed::PinId& key) const {
			return std::hash<ed::PinId>()(key);
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
	struct LinkInfo
	{
		ed::LinkId Id;
		ed::PinId  InputId;
		ed::PinId  OutputId;
	};

	ax::NodeEditor::EditorContext* m_nodeEditorContext{ nullptr };

	ed::EditorContext* m_Context = nullptr;    // Editor context, required to trace a editor state.
	bool                 m_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
	ImVector<LinkInfo>   m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
	int                  m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
	int m_uniqueId{ 1 };
	int m_linkId{ 1 };

	std::unordered_map<NodeLinkKey, NodeLinkData, NodeLinkKeyHasher> m_currentLinks{};
	std::unordered_map<ed::PinId, id, PinIdHasher> m_pinInIdToNodeId{};
	std::unordered_map<ed::PinId, id, PinIdHasher> m_pinOutIdToNodeId{};

	void OnStart()
	{
		ed::Config config;
		config.SettingsFile = "BasicInteraction.json";
		m_Context = ed::CreateEditor(&config);
	}

	void OnStop()
	{
		ed::DestroyEditor(m_Context);
	}

	void ImGuiEx_BeginColumn()
	{
		ImGui::BeginGroup();
	}

	void ImGuiEx_NextColumn()
	{
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
	}

	void ImGuiEx_EndColumn()
	{
		ImGui::EndGroup();
	}

public:
	VnEngineGraph() {
		OnStart();
	}

	~VnEngineGraph() {
		ax::NodeEditor::DestroyEditor(m_nodeEditorContext);
		OnStop();
	}

	void drawChapter(const Chapter* chapter, int x, int y) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
		std::string convertedName{ myconv.to_bytes(chapter->getName()) };

		ed::NodeId nodeId = chapter->getId();
		ed::PinId  inPinId = m_uniqueId++;
		ed::PinId  outPinId = m_uniqueId++;

		m_pinInIdToNodeId[inPinId] = chapter->getId();
		m_pinInIdToNodeId[outPinId] = chapter->getId();

		// Draw the node in the graph window
		if (m_FirstFrame)
			ed::SetNodePosition(nodeId, ImVec2(x, y));

		ed::BeginNode(nodeId);

		ImGui::Text(convertedName.c_str());

		ImGuiEx_BeginColumn();
		ed::BeginPin(inPinId, ed::PinKind::Input);
		ImGui::Text("-> In");
		ed::EndPin();
		ImGuiEx_EndColumn();

		ImGui::SameLine();

		ImGuiEx_BeginColumn();
		ed::BeginPin(outPinId, ed::PinKind::Output);
		ImGui::Text("Out ->");
		ed::EndPin();
		ImGuiEx_EndColumn();

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
			const Chapter* childChapter{ ModelSubject::getChapterById(chapterId) };

			if (childChapter != nullptr) {
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

	void draw() {

		const Chapter* headChapter{ ModelSubject::getHeadChapter() };

		auto& io = ImGui::GetIO();

		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

		ImGui::Separator();

		ed::SetCurrentEditor(m_Context);

		// Start interaction with editor.
		ed::Begin("My Editor", ImVec2(0.0, 0.0f));


		//
		// 1) Commit known data to editor
		//

		m_uniqueId = 1;
		m_linkId = 1;
		m_currentLinks.clear();
		m_pinInIdToNodeId.clear();
		m_pinOutIdToNodeId.clear();

		int x = 10;
		int y = 10;

		drawChapter(headChapter, x, y);


		// Submit Links
		//for (auto& linkInfo : m_Links)
			//ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
		
		for (auto& [key, data] : m_currentLinks) {
			ed::Link(data.m_id, data.m_inId, data.m_outId);
		}

		//
		// 2) Handle interactions
		//

		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ed::BeginCreate())
		{
			ed::PinId inputPinId, outputPinId;
			if (ed::QueryNewLink(&inputPinId, &outputPinId))
			{
				// QueryNewLink returns true if editor want to create new link between pins.
				//
				// Link can be created only for two valid pins, it is up to you to
				// validate if connection make sense. Editor is happy to make any.
				//
				// Link always goes from input to output. User may choose to drag
				// link from output pin or input pin. This determine which pin ids
				// are valid and which are not:
				//   * input valid, output invalid - user started to drag new ling from input pin
				//   * input invalid, output valid - user started to drag new ling from output pin
				//   * input valid, output valid   - user dragged link over other pin, can be validated

				if (inputPinId && outputPinId) // both are valid, let's accept link
				{
					// ed::AcceptNewItem() return true when user release mouse button.
					if (ed::AcceptNewItem())
					{
						// TODO: Error checking
					
						Chapter* parentNode{ ModelSubject::getChapterById(m_pinOutIdToNodeId[outputPinId]) };
						Chapter* childNode{ ModelSubject::getChapterById(m_pinInIdToNodeId[inputPinId]) };

						ChapterBuilder{ parentNode }.link(childNode);

						// Since we accepted new link, lets add one to our list of links.
						//m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						//ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
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
							m_currentLinks.erase(key);
							break;
						}
					}
				}

				// You may reject link deletion by calling:
				// ed::RejectDeletedItem();
			}
		}
		ed::EndDelete(); // Wrap up deletion action



		// End of interaction with editor.
		ed::End();

		if (m_FirstFrame)
			ed::NavigateToContent(0.0f);

		ed::SetCurrentEditor(nullptr);

		m_FirstFrame = false;
	}
};

#endif // VN_ENGINE_GRAPH_H