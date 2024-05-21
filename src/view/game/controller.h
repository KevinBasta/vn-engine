#ifndef VN_GAME_CONTROLLER_H
#define VN_GAME_CONTROLLER_H

#include "state_subject.h"




class GameController {
private:
	GLFWwindow* m_window{};
	StateSubject* m_stateSubject{};

public:
	GameController(GLFWwindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject }
	{
	}

	void processInput() {
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(m_window, true);
		}

		if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
			// reload all shaders?
		}

		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
			std::cout << "w pressed" << std::endl;
		}

		if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			std::cout << "LEFT MOUSE BUTTON PRESSED!" << std::endl;
		}

	}
};

#endif // VN_GAME_CONTROLLER_H
