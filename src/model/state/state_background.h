#ifndef VN_STATE_BACKGROUND_H
#define VN_STATE_BACKGROUND_H

#include "texture.h"

#include "state_types.h"
#include "model_subject.h"
#include "chapter_node_types.h"

class StateBackground {
public:
	Texture2D* m_currentBackground{ nullptr };

	void updateCurrentBackground(Texture2D* newBackground) {
		m_currentBackground = newBackground;
	}

	void handle(ActionBackgroundTexture& backgroundAction) {
		// TODO: Error handling
		m_currentBackground = ModelSubject::getBackgroundTexture(backgroundAction.backgroundIndex);

		//m_stateDelta.push_back(StateDelta::BACKGROUND);
	}
};

#endif // VN_STATE_BACKGROUND_H
