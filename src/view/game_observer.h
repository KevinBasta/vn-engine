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

#include <GLFW/glfw3.h>

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

			// Using a Child allow to fill all the space of the window.
			// It also alows customization
			//ImGui::BeginChild("GameRender");
			// Get the size of the child (i.e. the whole draw size of the windows).
			//ImVec2 wsize = ImGui::GetWindowSize();
			// Because I use the texture from OpenGL, I need to invert the V from the UV.
			//ImGui::Image((ImTextureID)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
			//ImGui::EndChild();
					
			ImGui::Begin("Test Window");

			const float newWidth = ImGui::GetContentRegionAvail().x;
			const float newHeight = ImGui::GetContentRegionAvail().y;

			std::pair<FrameDimensions, ViewportUpdateParams> windowSizeUpdate{};
			windowSizeUpdate = getNormalizedDimentions(newWidth, newHeight);
			glViewport(0, 0, windowSizeUpdate.first.width, windowSizeUpdate.first.height);
				
			unsigned int textureId = m_context.getTextureId();
			ImVec2 pos = ImGui::GetCursorScreenPos(); // screen position of the window

			// and here we can add our created texture as image to ImGui
			ImGui::GetWindowDrawList()->AddImage(
				(void*)textureId,
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + windowSizeUpdate.second.width, pos.y + windowSizeUpdate.second.height),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);

			m_context.renderEngine(windowSizeUpdate.first);

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
				//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				//glClear(GL_COLOR_BUFFER_BIT);

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
