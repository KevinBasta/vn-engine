#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "observer.h"
#include "context.h"
#include "state_subject.h"
#include "controller.h"
#include "text.h"
#include "engine_properties.h"
//#include "engine_observer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <GLFW/glfw3.h>

static void printTest() {
	std::cout << "imgui test" << std::endl;
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	//(*g_camera).processMouseMovement(xpos, ypos);
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//(*g_camera).processMouseScroll(yoffset);
}

class GameObserver : public Observer {
private:
	VnWindow* m_window;
	
	StateSubject* m_stateSubject{};
	GameContext m_context;
	GameController m_controller;
	bool m_rerenderContext{ false };
	//EngineObserver m_engine{};

public:
	// can register the callbacks here for controller
	GameObserver(VnWindow* window, StateSubject* stateSubjcet) :
		m_window{ window },
		m_stateSubject{ stateSubjcet },
		m_context{ window, stateSubjcet },
		m_controller{ window, stateSubjcet }
	{
		init();
		m_stateSubject->attatch(this);
	}

	void init() {
		//glfwSetInputMode(m_window->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glfwSetCursorPosCallback(m_window->get(), mouseCallback);
		//glfwSetScrollCallback(m_window->get(), scrollCallback);
	}

	void run() {
		if (IS_ENGINE_ACTIVE) {
			runEngine();
		}
		else {
			runRuntime();
		}
	}

	void runEngine() {
		// Init imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_window->get(), true);
		ImGui_ImplOpenGL3_Init("#version 150");

		// Gameloop
		double deltaTime = 0.0f;
		double lastFrame = 0.0f;

		while (!glfwWindowShouldClose(m_window->get())) {
			m_controller.processInput();

			double currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (m_window->updated()) {
				m_rerenderContext = true;
			}

			if (m_stateSubject->inAutoAction()) {
				m_stateSubject->tickAutoActions(deltaTime);
			}
			
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

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
					
			ImGui::Begin("Graph tree and Node step viewer");

			if (ImGui::TreeNode("TabItemButton & Leading/Trailing flags"))
			{
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
				ImGui::Separator();
				ImGui::TreePop();
			}

			ImGui::End();





			//
			// Game view window
			//

			ImGui::Begin("Viewport Preview");

			const float newWidth = ImGui::GetContentRegionAvail().x;
			const float newHeight = ImGui::GetContentRegionAvail().y;

			FrameDimensions windowSizeUpdate{ getNormalizedDimentions(newWidth, newHeight) };
			glViewport(0, 0, windowSizeUpdate.width, windowSizeUpdate.height);
				
			unsigned int textureId = m_context.getTextureId();
			ImVec2 pos = ImGui::GetCursorScreenPos(); // screen position of the window

			// and here we can add our created texture as image to ImGui
			ImGui::GetWindowDrawList()->AddImage(
				(void*)textureId,
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + windowSizeUpdate.width, pos.y + windowSizeUpdate.height),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);

			m_context.renderEngine(windowSizeUpdate);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Fullscreen", NULL, &printTest);
					ImGui::MenuItem("Padding", NULL, &printTest);
					ImGui::Separator();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::End();




			ImGui::ShowDemoWindow();



			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			m_stateSubject->clearStateDelta();

			// Update and Render additional Platform Windows
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			glfwSwapBuffers(m_window->get());
			glfwPollEvents();
		}

		uninit();

		// uninit imgui
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}


	void runRuntime() {
		// Gameloop
		double deltaTime = 0.0f;
		double lastFrame = 0.0f;

		while (!glfwWindowShouldClose(m_window->get())) {
			m_controller.processInput();

			double currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (m_window->updated()) {
				m_rerenderContext = true;
			}

			if (m_rerenderContext || m_stateSubject->inAutoAction()) {
				// Clear the screen content
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


				if (m_stateSubject->inAutoAction()) {
					m_stateSubject->tickAutoActions(deltaTime);
				}

				m_context.renderRuntime();


				glfwSwapBuffers(m_window->get());

				m_rerenderContext = false;
				m_stateSubject->clearStateDelta();
			}
		
			glfwPollEvents();
		}

		uninit();
	}

	void uninit() {
		// TODO: destroy glfw window
	}

	void update() {
		std::cout << "GAME OBSERVER UPDATE CALLED" << std::endl;
		m_rerenderContext = true;
	}
};

#endif // GAME_OBSERVER_H
