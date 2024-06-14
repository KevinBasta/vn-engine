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
	VnText m_text{};
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
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		while (!glfwWindowShouldClose(m_window->get())) {
			m_controller.processInput();

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (m_window->updated()) {
				m_rerenderContext = true;
			}

			if (m_rerenderContext) {
			
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				

				m_context.draw();

				m_text.RenderText(L"test test test lol 123 あ私", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
				m_text.RenderText(L"test test test lol 123 あ私", 1.0f, 1.0f, 1.0f, glm::vec3(0.2, 0.9f, 0.7f));
				m_text.RenderText(L"test test test lol 123 あ私", 55.0f, 55.0f, 1.0f, glm::vec3(0.5, 0.2f, 0.2f));
			
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
