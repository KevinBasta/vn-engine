#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "observer.h"
#include "context.h"
#include "model_subject.h"
//#include "state_subject.h"

class GameObserver : Observer {
private:
	ModelSubject* m_modelSubject{};
	//StateSubject* m_stateSubject{};
	GameContext m_vn{};

public:
	GameObserver(ModelSubject* modelSubjcet) : m_modelSubject{ modelSubjcet }
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
