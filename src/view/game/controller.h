#ifndef VN_GAME_CONTROLLER_H
#define VN_GAME_CONTROLLER_H

#include "state_subject.h"


static bool m_leftButtonReleaseEvent{ false };


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		m_leftButtonReleaseEvent = true;
	}
}


class GameController {
private:
	GLFWwindow* m_window{};
	StateSubject* m_stateSubject{};

public:
	GameController(GLFWwindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject }
	{
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
	}

	void processInput() {
		processMouse();
		processKeyboard();
	}

private:
	void processMouse() {
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
			std::cout << "w pressed" << std::endl;
		}

		if (m_leftButtonReleaseEvent) {
			m_stateSubject->updateCurrentText("lol", "test");
			m_leftButtonReleaseEvent = false;
		}

		//if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		//	// Move state forward
		//	std::cout << "LEFT MOUSE BUTTON PRESSED!" << std::endl;
		//}

		//if (m_leftMouseButtonWasPressedLastFrame && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		//	m_stateSubject->updateCurrentText("lol", "test");
		//}
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
};

#endif // VN_GAME_CONTROLLER_H
