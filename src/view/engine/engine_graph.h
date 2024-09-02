#ifndef VN_ENGINE_GRAPH_H
#define VN_ENGINE_GRAPH_H

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

#include <GLFW/glfw3.h>

namespace ed = ax::NodeEditor;

class VnEngineGraph {
private:

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
		ed::PinId  inPinIdStart = chapter->getId() + 10000;
		ed::PinId  outPinIdStart = chapter->getId() + 20000;

		if (m_FirstFrame)
			ed::SetNodePosition(nodeId, ImVec2(10, 10));

		ed::BeginNode(nodeId);
		ImGui::Text(convertedName.c_str());

		ImGuiEx_BeginColumn();
		for (int i{ 0 }; i <= chapter->getParentsAmount(); i++) {
			ed::BeginPin(inPinIdStart, ed::PinKind::Input);
			ImGui::Text("-> In");
			ed::EndPin();

			inPinIdStart = inPinIdStart.Get() + 1;
		}
		ImGuiEx_NextColumn();

		ImGui::SameLine();

		for (int i{ 0 }; i <= chapter->getChildrenAmount(); i++) {
			ed::BeginPin(outPinIdStart, ed::PinKind::Output);
			ImGui::Text("Out ->");
			ed::EndPin();

			outPinIdStart = outPinIdStart.Get() + 1;
		}
		ImGuiEx_EndColumn();

		ed::EndNode();
	}

	void draw() {

		const Chapter* headChapter{ ModelSubject::getHeadChapter() };

		auto& io = ImGui::GetIO();

		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

		ImGui::Separator();

		ed::SetCurrentEditor(m_Context);

		// Start interaction with editor.
		ed::Begin("My Editor", ImVec2(0.0, 0.0f));

		int uniqueId = 1;

		//
		// 1) Commit known data to editor
		//

		drawChapter(headChapter, 1, 1);


		// Submit Links
		for (auto& linkInfo : m_Links)
			ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

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
						// Since we accepted new link, lets add one to our list of links.
						m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
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
					for (auto& link : m_Links)
					{
						if (link.Id == deletedLinkId)
						{
							m_Links.erase(&link);
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