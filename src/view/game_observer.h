#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "observer.h"
#include "context.h"
#include "state_subject.h"
#include "controller.h"
#include "text.h"

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

public:
	// can register the callbacks here for controller
	GameObserver(VnWindow* window, StateSubject* stateSubjcet) :
		m_window{ window },
		m_stateSubject{ stateSubjcet },
		m_context{ window, stateSubjcet },
		m_controller{ window, stateSubjcet }
	{
	}

	void init() {
		glfwSetInputMode(m_window->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(m_window->get(), mouseCallback);
		glfwSetScrollCallback(m_window->get(), scrollCallback);

	}

	void run() {
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
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				if (m_stateSubject->inAutoAction()) {
					m_stateSubject->tickAutoActions(deltaTime);
				}

				m_context.draw();

				glfwSwapBuffers(m_window->get());

				m_rerenderContext = false;
				m_stateSubject->clearStateDelta();
			}
		
			glfwPollEvents();
		}
		
	}

	void update() {
		std::cout << "GAME OBSERVER UPDATE CALLED" << std::endl;
		m_rerenderContext = true;
	}
};

#endif // GAME_OBSERVER_H
