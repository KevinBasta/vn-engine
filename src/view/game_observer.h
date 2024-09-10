#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "observer.h"
#include "context.h"
#include "state_subject.h"
#include "controller.h"
#include "text.h"
#include "engine_properties.h"
//#include "engine_observer.h"
#include "engine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node_editor.h"

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
		VnEngine m_engine{ m_window, m_stateSubject, &m_context };

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

			m_engine.draw();
			
			m_stateSubject->clearStateDelta();

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
