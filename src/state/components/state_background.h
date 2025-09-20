#ifndef VN_STATE_BACKGROUND_H
#define VN_STATE_BACKGROUND_H

#include "texture.h"

#include "state_types.h"
#include "model_subject.h"
#include "model_runtime_interface.h"
#include "node_types.h"

class StateBackground {
public:
	TextureIdentifier m_currentBackground{};
	BackgroundOffsets m_positionOffsets{}; // Offsets to centered screen positions
	
	// TODO: allow background color setting
	// TODO: allow background animations

	void handle(const ActionBackgroundTexture& action) {
		// TODO: Error handling
		
		// Set the texture
		m_currentBackground = action.texture;

		// Reset position offsets
		m_positionOffsets = BackgroundOffsets{};

		// Set position offsets
		if (action.xposEnabled) {
			m_positionOffsets.xpos = action.xpos;
		}

		if (action.yposEnabled) {
			m_positionOffsets.ypos = action.ypos;
		}

		if (action.zposEnabled) {
			m_positionOffsets.zpos = action.zpos;
		}

		if (action.scaleEnabled) {
			m_positionOffsets.scale = action.scale;
		}

		if (action.rotationEnabled) {
			m_positionOffsets.rotation = action.rotation;
		}

		if (action.opacityEnabled) {
			m_positionOffsets.opacity = action.opacity;
		}

		// TODO: have load texture return a true if success
		// if fail then fall back to a base image for background
		// or a solid color?
		ModelRuntimeInterface::loadTexture(action.texture);

		//m_stateDelta.push_back(StateDelta::BACKGROUND);
	}
};

#endif // VN_STATE_BACKGROUND_H
