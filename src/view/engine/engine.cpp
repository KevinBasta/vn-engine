
#include "engine.h"

#include "window.h"
#include "context.h"
#include "text.h"
#include "state_subject.h"

#include "engine_chapter_graph.h"
#include "engine_node_graph.h"
#include "engine_preview.h"
#include "engine_node_editor.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <GLFW/glfw3.h>

VnEngine::VnEngine(VnWindow* window, StateSubject* stateSubject, GameContext* context) :
	m_window{ window },
	m_context{ context },
	m_stateSubject{ stateSubject },
	m_engineNodeGraph{ stateSubject },
	m_engineChapterGraph{ stateSubject },
	m_engineStepTimeline{ stateSubject },
	m_enginePreview{ context },
	m_engineNodeEditor{ stateSubject }
{
	initImgui();
}

bool VnEngine::framebufferFocused() {
	// TODO: framebuffer window can be a class member variable
	auto framebufferWindow{ ImGui::FindWindowByName(WINDOW_VIEWPORT_PREVIEW) };
	auto currentWindow{ ImGui::GetCurrentContext()->NavWindow };
	return  (currentWindow != nullptr) && (framebufferWindow == currentWindow) ;
}

void VnEngine::initImgui() {
	// Init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	// TODO: DECIDE ON THE RAGE TO ALWAYS LOAD https://github.com/ocornut/imgui/blob/master/docs/FONTS.md#about-utf-8-encoding
	bool fontLoad = io.Fonts->AddFontFromFileTTF(TEMP_FONT_PATH, 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	if (fontLoad) { std::cout << "FONT LOAD SUCCESS" << std::endl; }

	// Don't use saved state
	io.IniFilename = NULL;

	// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window->get(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void VnEngine::createDockspace() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (m_resetDockspace)
		{
			m_resetDockspace = false;

			ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
			ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			if (m_expandedDockingView) {
				ImGui::DockBuilderDockWindow(WINDOW_VIEWPORT_PREVIEW, dockspace_id);
				ImGui::DockBuilderDockWindow(WINDOW_CHAPTER_GRAPH, dockspace_id);
				ImGui::DockBuilderDockWindow(WINDOW_NODE_GRAPH, dockspace_id);
				ImGui::DockBuilderDockWindow(WINDOW_STEP_TIMELINE, dockspace_id);
				ImGui::DockBuilderDockWindow(WINDOW_NODE_EDITOR, dockspace_id);
				//ImGui::DockBuilderDockWindow(WINDOW_IMGUI_DEMO, dockspace_id);
			}
			else {
				ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.4f, nullptr, &dockspace_id);

				// The order of these two decides which one expands when the OpenGL window is put in full screen
				ImGuiID dock_id_up = 0;
				ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.5f, nullptr, &dock_id_up);

				// we now dock our windows into the docking node we made above
				ImGui::DockBuilderDockWindow(WINDOW_VIEWPORT_PREVIEW, dock_id_up);
				ImGui::DockBuilderDockWindow(WINDOW_CHAPTER_GRAPH, dock_id_down);
				ImGui::DockBuilderDockWindow(WINDOW_NODE_GRAPH, dock_id_down);
				ImGui::DockBuilderDockWindow(WINDOW_STEP_TIMELINE, dock_id_down);
				ImGui::DockBuilderDockWindow(WINDOW_NODE_EDITOR, dock_id_left);
				//ImGui::DockBuilderDockWindow(WINDOW_IMGUI_DEMO, dock_id_left);
			}

			ImGui::DockBuilderFinish(dockspace_id);
		}
	}
}

void VnEngine::createMenuBar() {
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Main Menu"))
		{
			// TODO: implement shortcuts
			if (ImGui::MenuItem("New Project")) {

			}

			if (ImGui::MenuItem("Open Project", "Ctrl+O")) {

			}

			if (ImGui::MenuItem("Save Project", "Ctrl+S")) {

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Layout")) {
			if (ImGui::MenuItem("reset layout")) {
				m_resetDockspace = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Preview Mode", NULL, &m_expandedDockingView, true)) {
				m_resetDockspace = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}


void VnEngine::draw() {
	createDockspace();
	createMenuBar();

	if (ImGui::Begin(WINDOW_VIEWPORT_PREVIEW)) {
		m_enginePreview.draw();
	}
	ImGui::End();

	if (ImGui::Begin(WINDOW_CHAPTER_GRAPH)) {
		m_engineChapterGraph.draw();
	}
	ImGui::End();

	if (ImGui::Begin(WINDOW_NODE_GRAPH)) {
		m_engineNodeGraph.draw();
	}
	ImGui::End();

	if (ImGui::Begin(WINDOW_STEP_TIMELINE)) {
		m_engineStepTimeline.draw();
	}
	ImGui::End();

	if (ImGui::Begin(WINDOW_NODE_EDITOR)) {
		m_engineNodeEditor.draw();
	}
	ImGui::End();


	//ImGui::ShowDemoWindow();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	if (m_firstDraw) {
		m_firstDraw = false;
	}
}




/*
	if (ImGui::Begin("BROWSE")) {

	}
	ImGui::End();

	if (ImGui::Begin("CHAPTER EDITOR")) {

	}
	ImGui::End();


	if (ImGui::Begin("TEXTURE STORES")) {

	}
	ImGui::End();

	if (ImGui::Begin("SOUND STORES")) {

	}
	ImGui::End();



	ImGui::DockBuilderDockWindow("BROWSE", dock_id_left);
	ImGui::DockBuilderDockWindow("CHAPTER EDITOR", dock_id_left);
	ImGui::DockBuilderDockWindow("TEXTURE STORES", dock_id_left);
	ImGui::DockBuilderDockWindow("SOUND STORES", dock_id_left);

	ImGui::DockBuilderDockWindow("BROWSE", dockspace_id);
	ImGui::DockBuilderDockWindow("CHAPTER EDITOR", dockspace_id);
	ImGui::DockBuilderDockWindow("TEXTURE STORES", dockspace_id);
	ImGui::DockBuilderDockWindow("SOUND STORES", dockspace_id);


*/