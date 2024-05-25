#ifndef VN_GAME_CONTROLLER_H
#define VN_GAME_CONTROLLER_H

#include "state_subject.h"




class GameController {
private:
	GLFWwindow* m_window{};
	StateSubject* m_stateSubject{};

	void processMouse() {
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
			std::cout << "w pressed" << std::endl;
		}

		if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			// Move state forward
			std::cout << "LEFT MOUSE BUTTON PRESSED!" << std::endl;
		}
	}

	void processKeyboard() {
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			// open menu?
			glfwSetWindowShouldClose(m_window, true);
		}

		if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
			// development: reload all shaders?
			// TODO: implement when needed
		}
	}

public:
	GameController(GLFWwindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject }
	{
	}

	void processInput() {
		processMouse();
		processKeyboard();
	}
};

#endif // VN_GAME_CONTROLLER_H
