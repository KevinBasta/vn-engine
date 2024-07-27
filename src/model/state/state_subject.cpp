
#include "state_subject.h"
#include "texture_manager.h"



void StateSubject::tickAutoActions(float timePassed) {
	bool anyActive{ false };

	anyActive |= m_sprites.tickAutoActions(timePassed);
	
	// Check if any auto actions ran this tick. If none, then clear the auto action.
	if (anyActive == false) {
		clearAutoAction();
	}
}


void StateSubject::endAutoActions() {
	// Go to end of all auto actions
	m_sprites.endAutoActions();
}
