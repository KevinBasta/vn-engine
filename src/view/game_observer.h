#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "observer.h"
#include "context.h"
#include "state_subject.h"
#include "controller.h"

#include <GLFW/glfw3.h>


static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	//(*g_camera).processMouseMovement(xpos, ypos);
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//(*g_camera).processMouseScroll(yoffset);
}

class GameObserver : Observer {
private:
	GLFWwindow* m_window;
	
	StateSubject* m_stateSubject{};
	GameContext m_context;
	GameController m_controller;

public:
	// can register the callbacks here for controller
	GameObserver(GLFWwindow* window, StateSubject* stateSubjcet) :
		m_window{ window },
		m_stateSubject{ stateSubjcet },
		m_context{ window },
		m_controller{ window, stateSubjcet }
	{
	}

	void init() {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(m_window, mouseCallback);
		glfwSetScrollCallback(m_window, scrollCallback);

	}

	void run() {
		// Gameloop
		
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		while (!glfwWindowShouldClose(m_window)) {
			m_controller.processInput();

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;


			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//for (int i)
			m_context.drawBackground(m_stateSubject->m_currentBackground);
			m_context.drawSprite(m_stateSubject->m_tempTexture);

		
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
		
	}

	void update() {
		std::cout << "GAME OBSERVER UPDATE CALLED" << std::endl;
	}
};

#endif // GAME_OBSERVER_H
