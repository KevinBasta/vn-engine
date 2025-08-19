#ifndef VN_GAME_CONTROLLER_H
#define VN_GAME_CONTROLLER_H

#include "window.h"
#include "state_subject.h"
#include "engine_properties.h"
#include "controller_helpers.h"

static bool sg_leftButtonReleaseEvent{ false };
static bool sg_upKeyButtonReleaseEvent{ false };
static bool sg_downKeyButtonReleaseEvent{ false };
static bool sg_enterKeyButtonReleaseEvent{ false };

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		sg_leftButtonReleaseEvent = true;
	}
}


void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Can use GLFW_PRESS here because after first call it will be GLFW_REPEAT
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		sg_upKeyButtonReleaseEvent = true;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		sg_downKeyButtonReleaseEvent = true;
	}
	
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		sg_enterKeyButtonReleaseEvent = true;
	}
}

class GameController {
private:
	VnWindow* m_window{};
	StateSubject* m_stateSubject{};

public:
	GameController(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject }
	{
		glfwSetMouseButtonCallback(window->get(), mouseButtonCallback);
		glfwSetKeyCallback(window->get(), keyPressCallback);
	}

	void processInput(bool processInput = true) {
		if (processInput) {
			processMouse();
			processKeyboard();
		}
		else {
			discardMouse();
			discardKeyboard();
		}
	}

private:
	void discardMouse() {
		if (sg_leftButtonReleaseEvent) {
			sg_leftButtonReleaseEvent = false;
		}
	}

	void processMouse() {
		// Do a state step on left click
		if (sg_leftButtonReleaseEvent) {
			if (m_stateSubject->inMainMenu()) {
				
			}
			else if (m_stateSubject->m_choices.isChoiceActive() == false) {
				m_stateSubject->action();
			}

			sg_leftButtonReleaseEvent = false;
		}

		// TODO: to add support for clicking the node children choices, need to
		// 1. save the poisition/bounds in the choice layer after each redraw
		// 2. give the controller a pointer to the context or the choice layer to get those positions/bounds

		//if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		//}

		//if (m_leftMouseButtonWasPressedLastFrame && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		//}
	}

	void discardKeyboard() {
		if (sg_enterKeyButtonReleaseEvent) {
			sg_enterKeyButtonReleaseEvent = false;
		}

		if (sg_upKeyButtonReleaseEvent) {
			sg_upKeyButtonReleaseEvent = false;
		}

		if (sg_downKeyButtonReleaseEvent) {
			sg_downKeyButtonReleaseEvent = false;
		}
	}

	void processKeyboard() {

		if (/*!IS_ENGINE_ACTIVE && */glfwGetKey(m_window->get(), GLFW_KEY_ESCAPE) == GLFW_PRESS || m_stateSubject->inQuitState()) {
			// TODO: open menu?
			glfwSetWindowShouldClose(m_window->get(), true);
		}

		// Do a state step on enter press (picking a node child if choice is active)
		if (sg_enterKeyButtonReleaseEvent) {
			m_stateSubject->action();
			sg_enterKeyButtonReleaseEvent = false;
		}
		

		// Change node child choice index with up and down arrows
		if (sg_upKeyButtonReleaseEvent) {
			if (m_stateSubject->inMainMenu()) {
				m_stateSubject->m_mainMenu.chooseUpChoice();;
			} else if (m_stateSubject->m_choices.isChoiceActive()) {
				m_stateSubject->m_choices.chooseUpChoice();
			}
			
			sg_upKeyButtonReleaseEvent = false;
		}

		if (sg_downKeyButtonReleaseEvent) {
			if (m_stateSubject->inMainMenu()) {
				m_stateSubject->m_mainMenu.chooseDownChoice();
			}
			else if (m_stateSubject->m_choices.isChoiceActive()) {
				m_stateSubject->m_choices.chooseDownChoice();
			}
			
			sg_downKeyButtonReleaseEvent = false;
		}





		if (glfwGetKey(m_window->get(), GLFW_KEY_W) == GLFW_PRESS) {
			std::cout << "w pressed" << std::endl;
		}

		if (glfwGetKey(m_window->get(), GLFW_KEY_R) == GLFW_PRESS) {
			// development: reload all shaders?
			// TODO: implement when needed
		}
	}
};

#endif // VN_GAME_CONTROLLER_H
