#ifndef VN_ENGINE_GUI_H
#define VN_ENGINE_GUI_H

#include "window.h"
#include "context.h"
#include "state_subject.h"

#include "engine_chapter_graph.h"
#include "engine_node_graph.h"
#include "engine_preview.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

#include <GLFW/glfw3.h>

static void printTest() {
	std::cout << "imgui test" << std::endl;
}

class VnEngine {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	GameContext* m_context{ nullptr };
	bool m_firstDraw{ true };
	VnEnginePreview m_enginePreview;
	VnEngineChapterGraph m_engineChapterGraph;
	VnEngineNodeGraph m_engineNodeGraph;

	void initImgui() {
		// Init imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		
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

public:
	VnEngine(VnWindow* window, StateSubject* stateSubject, GameContext* context) :
		m_window{ window },
		m_context{ context },
		m_stateSubject{ stateSubject },
		m_engineNodeGraph{ stateSubject },
		m_engineChapterGraph{ stateSubject },
		m_enginePreview{ context }
	{
		initImgui();
	}

	~VnEngine() {

	}

	void draw() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

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

			static auto first_time = true;
			if (first_time)
			{
				first_time = false;

				ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
				ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

				// split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
				//   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
				//                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
				auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.4f, nullptr, &dockspace_id);
				//ImVec2 dockspace_size = ImGui::GetContentRegionAvail();
				//ImGui::DockBuilderSetNodeSize(dockspace_id, dockspace_size);

				auto dock_id_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.5f, nullptr, &dockspace_id);
				//https://github.com/ocornut/imgui/issues/6217
				//dockspace_size = ImGui::GetContentRegionAvail();
				//ImGui::DockBuilderSetNodeSize(dockspace_id, dockspace_size);

				auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 1.0f, nullptr, &dockspace_id);
				
				// we now dock our windows into the docking node we made above
				ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_id_left);
				ImGui::DockBuilderDockWindow("Viewport Preview", dock_id_up);
				ImGui::DockBuilderDockWindow("CHAPTERS", dock_id_down);
				ImGui::DockBuilderDockWindow("NODES", dock_id_down);

				ImGui::DockBuilderFinish(dockspace_id);
			}
		}

		//https://github.com/ocornut/imgui/blob/docking/imgui_internal.h#L3657

		ImGui::End();

		ImGui::Begin("Viewport Preview");
		m_enginePreview.draw();
		ImGui::End();
		
		ImGui::Begin("CHAPTERS");
		m_engineChapterGraph.draw();
		ImGui::End();

		ImGui::Begin("NODES");
		m_engineNodeGraph.draw();
		ImGui::End();

		ImGui::ShowDemoWindow();

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


};


static void tabs() {
	/*ImGui::Begin("");
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("TESTETSETSETSETSETSET"))
			{
				ImGui::MenuItem("Main menu bar", NULL, &printTest);

				ImGui::SeparatorText("Mini apps");
				ImGui::MenuItem("Assets Browser", NULL, &printTest);
				ImGui::MenuItem("Console", NULL, &printTest);

				ImGui::SeparatorText("Concepts");
				ImGui::MenuItem("Auto-resizing window", NULL, &printTest);
				ImGui::MenuItem("Auto-resizing window", NULL, &printTest);
				ImGui::MenuItem("Auto-resizing window", NULL, &printTest);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();*/


		// Using a Child allow to fill all the space of the window.
		// It also alows customization
		//ImGui::BeginChild("GameRender");
		// Get the size of the child (i.e. the whole draw size of the windows).
		//ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		//ImGui::Image((ImTextureID)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::EndChild();

		//ImGui::Begin("Graph tree and Node step viewer");

		/*
		static ImVector<int> active_tabs;
		static int next_tab_id = 0;
		if (next_tab_id == 0) // Initialize with some default tabs
			for (int i = 0; i < 3; i++)
				active_tabs.push_back(next_tab_id++);

			// TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
			// (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
			// but they tend to make more sense together)
			static bool show_leading_button = true;
			static bool show_trailing_button = true;
			ImGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
			ImGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

			// Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
			static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
			ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
			if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
				tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
			if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
				tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				// Demo a Leading TabItemButton(): click the "?" button to open a menu
				if (show_leading_button)
					if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
						ImGui::OpenPopup("MyHelpMenu");
				if (ImGui::BeginPopup("MyHelpMenu"))
				{
					ImGui::Selectable("Hello!");
					ImGui::EndPopup();
				}

				// Demo Trailing Tabs: click the "+" button to add a new tab.
				// (In your app you may want to use a font icon instead of the "+")
				// We submit it before the regular tabs, but thanks to the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
				if (show_trailing_button)
					if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
						active_tabs.push_back(next_tab_id++); // Add new tab

				// Submit our regular tabs
				for (int n = 0; n < active_tabs.Size; )
				{
					bool open = true;
					char name[16];
					snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
					if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
					{
						ImGui::Text("This is the %s tab!", name);
						ImGui::EndTabItem();
					}

					if (!open)
						active_tabs.erase(active_tabs.Data + n);
					else
						n++;
				}

				ImGui::EndTabBar();
			}
			*/
}

#endif // VN_ENGINE_GUI_H
