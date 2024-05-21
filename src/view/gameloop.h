#ifndef VN_GAMELOOP_H
#define VN_GAMELOOP_H

#include "game_observer.h"
#include "engine_observer.h"

class VnGameloop {
private:
	GameObserver* m_gameObserver{};
	EngineObserver* m_engineObserver{};

public:
	VnGameloop() {

	}

	void gameLoop() {
		while (1) {

		}
	}
};

#endif // VN_GAMELOOP_H
