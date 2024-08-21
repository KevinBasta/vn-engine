#ifndef ENGINE_OBSERVER_H
#define ENGINE_OBSERVER_H

/*

#include "observer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

// Can have all the functinallity to change values at differnet parts of the program.
// Can be a friend class to most classes
class EngineObserver : Observer {
private:
	GLFWwindow* m_window{};

public:
	EngineObserver(GLFWwindow* window) :
		m_window{ window }
	{
		init();
	}

	void init() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 150");
	}

	void tick() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void uninit() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void update() {
		std::cout << "ENGINE OBSERVER UPDATE CALLED" << std::endl;
	}
};

*/

#endif // ENGINE_OBSERVER_H
