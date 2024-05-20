#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "observer.h"
#include "context.h"
#include "model_subject.h"
//#include "state_subject.h"

#include <GLFW/glfw3.h>

class GameObserver : Observer {
private:
	ModelSubject* m_modelSubject{};
	GLFWwindow* m_window;
	//StateSubject* m_stateSubject{};
	GameContext m_vn;

public:
	// can register the callbacks here for controller
	GameObserver(GLFWwindow* window, ModelSubject* modelSubjcet) : 
		m_window{ window },
		m_modelSubject{ modelSubjcet },
		m_vn{ window }
	{
		
	}

	void run() {
		m_vn.addSubject(m_modelSubject);
		m_vn.gameLoop();
	}

	void update() {

	}
};

#endif // GAME_OBSERVER_H
