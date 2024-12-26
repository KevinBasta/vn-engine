#ifndef VN_STATE_BACKGROUND_H
#define VN_STATE_BACKGROUND_H

#include "texture.h"

#include "state_types.h"
#include "model_subject.h"
#include "model_runtime_interface.h"
#include "chapter_node_types.h"

class StateBackground {
public:
	TextureIdentifier m_currentBackground{};

	// TODO: allow background scalling, moving, rotating, etc..
	// TODO: allow background color setting

	void handle(const ActionBackgroundTexture& backgroundAction) {
		// TODO: Error handling
		m_currentBackground = backgroundAction.m_texture;

		// TODO: have load texture return a true if success
		// if fail then fall back to a base image for background
		// or a solid color?
		ModelRuntimeInterface::loadTexture(backgroundAction.m_texture);

		//m_stateDelta.push_back(StateDelta::BACKGROUND);
	}
};

#endif // VN_STATE_BACKGROUND_H
