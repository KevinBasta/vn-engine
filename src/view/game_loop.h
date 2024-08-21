#ifndef VN_GAMELOOP_H
#define VN_GAMELOOP_H

/*
#include "window.h"

#include "game_observer.h"
#include "engine_observer.h"

#include <memory>

class GameLoop {
private:
	VnWindow* m_window{};
	std::unique_ptr<GameObserver> m_gameObserver{};
	std::unique_ptr<EngineObserver> m_engineObserver{};

public:
	GameLoop(VnWindow* window, StateSubject* stateSubject):
		m_window{ window },
		m_gameObserver{ new GameObserver{ window, stateSubject } },
		m_engineObserver{ new EngineObserver{ window->get() } }
	{
	}

	void run() {
		while (!glfwWindowShouldClose(m_window->get())) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_gameObserver->tick();
			m_engineObserver->tick();

			glfwSwapBuffers(m_window->get());
			glfwPollEvents();
		}
	
		m_gameObserver.get()->uninit();
		m_engineObserver.get()->uninit();
	}
};
*/

#endif // VN_GAMELOOP_H
