
#include "state_subject.h"
#include "state_types.h"
#include "model_subject.h"
#include "model_runtime_interface.h"
#include "state_sprites.h"
#include "chapter_node_types.h"


//
// SPRITE PROPERTIES
//
//

void StateSprites::handle(const ActionSpriteAllProperties& action) {
	if (m_spriteRenderData.find(action.m_texture) == m_spriteRenderData.end()) {
		// TODO: handle error/exceptions

		// TODO: should propergate error from load texture!
		ModelRuntimeInterface::loadTexture(action.m_texture);
	}

	if (action.m_xposEnabled) {
		m_spriteRenderData[action.m_texture].m_xpos = action.m_xpos;
	}

	if (action.m_yposEnabled) {
		m_spriteRenderData[action.m_texture].m_ypos = action.m_ypos;
	}

	if (action.m_zposEnabled) {
		m_spriteRenderData[action.m_texture].m_zpos = action.m_zpos;
	}

	if (action.m_scaleEnabled) {
		m_spriteRenderData[action.m_texture].m_scale = action.m_scale;
	}
	
	if (action.m_rotationEnabled) {
		m_spriteRenderData[action.m_texture].m_rotation = action.m_rotation;
	}

	if (action.m_opacityEnabled) {
		m_spriteRenderData[action.m_texture].m_opacity = action.m_opacity;
	}

}



//
// SPRITE ANIMATIONS
//
//

void StateSprites::handle(const ActionSpriteAnimation& action)
{
	if (m_stateSubject != nullptr) {
		m_stateSubject->setAutoAction();
	}

	std::cout << "auto action handling" << std::endl;
	m_activeSpriteAnimations.emplace_back(std::pair{ 0, action });
}

bool StateSprites::tickAutoActions(float timePassed) {
	bool anyActive{ false };

	anyActive |= tickSpriteAnimations(timePassed);

	return anyActive;
}

// Return true if animations are done, false otherwise
bool StateSprites::tickSpriteAnimations(float timePassed) {
	bool animationsActive{ false };

	auto animation{ m_activeSpriteAnimations.begin() };
	for (; animation != m_activeSpriteAnimations.end();) {
		auto& obj{ *animation };
		auto& properties{ obj.second };

		tick(obj, timePassed);

		bool active{ 
			properties.m_xposEnabled || properties.m_yposEnabled || properties.m_zposEnabled || 
			properties.m_scaleEnabled || properties.m_rotationEnabled || properties.m_opacityEnabled 
		};

		if (!active) {
			// Iterator invalidated when item erased, so go to next item
			std::cout << "ERASED AN ENTRY" << std::endl;
			animation = m_activeSpriteAnimations.erase(animation);
			if (animation == m_activeSpriteAnimations.end()) {
				break;
			}
		}
		else {
			// Go to next item manually
			animationsActive = true;
			animation++;
		}
	}

	return animationsActive;
}

// TODO: this is a little jerkey at the end of scale animations. perhaps allow time curve options.
// return true if animation is done, false otherwise
bool StateSprites::tick(std::pair<StepIndices, ActionSpriteAnimation>& animation, float timePassed) {
	bool active{ false };
	StepIndices& indicies{ animation.first };
	ActionSpriteAnimation& properties{ animation.second };
	SpriteState& state{ m_spriteRenderData[properties.m_texture] };

	if (properties.m_xposEnabled) {
		tickPropertyAnimation(properties.m_xposEnabled, timePassed, indicies.x, properties.m_xpos, state.m_xpos);
		active = true;
	}

	if (properties.m_yposEnabled) {
		tickPropertyAnimation(properties.m_yposEnabled, timePassed, indicies.y, properties.m_ypos, state.m_ypos);
		active = true;
	}

	if (properties.m_zposEnabled) {
		tickPropertyAnimation(properties.m_zposEnabled, timePassed, indicies.z, properties.m_zpos, state.m_zpos);
		active = true;
	}

	if (properties.m_scaleEnabled) {
		tickPropertyAnimation(properties.m_scaleEnabled, timePassed, indicies.scale, properties.m_scale, state.m_scale);
		active = true;
	}

	if (properties.m_rotationEnabled) {
		tickPropertyAnimation(properties.m_rotationEnabled, timePassed, indicies.rotation, properties.m_rotation, state.m_rotation);
		active = true;
	}

	if (properties.m_opacityEnabled) {
		tickPropertyAnimation(properties.m_opacityEnabled, timePassed, indicies.opacity, properties.m_opacity, state.m_opacity);
		active = true;
	}

	return active;
}

bool StateSprites::tickPropertyAnimation(bool& enabled, float timePassed, index& stepIndex, std::vector<SpriteAnimationKeyframe>& keyframes, float& currentState) {
	if (stepIndex > keyframes.size() - 1 || keyframes.size() == 0 || enabled == false) {
		enabled = false;
		return false;
	}

	// TODO: error when engine modifies this
	auto& currentAction{ keyframes[stepIndex] };
	float goalValue = (currentAction.m_value);

	float fractionOfTimePassed = currentAction.m_transitionSeconds / timePassed;

	float delta = (std::max(currentState, goalValue) - std::min(currentState, goalValue)) / fractionOfTimePassed;

	if (currentState < goalValue) {
		if (currentState + delta > goalValue) {
			currentState = goalValue;
		}
		else {
			currentState += delta;
		}
	}
	else {
		if (currentState - delta < goalValue) {
			currentState = goalValue;
		}
		else {
			currentState -= delta;
		}
	}

	currentAction.m_transitionSeconds -= timePassed;

	// If the character value state is close enough to the goal value and the transition seconds left is low
	// Then set the character value state to the goal state and set the goal to the next step or end the animation
	if (std::abs(currentState - currentAction.m_value) <= 5.0f && currentAction.m_transitionSeconds < 0.1f) {
		currentState = currentAction.m_value;

		if (stepIndex + 1 > keyframes.size() - 1) {
			enabled = false;
			return false;
		}
		else {
			stepIndex++;
		}
	}

	return true;
}

void StateSprites::endPropertyAnimation(bool& enabled, index& stepIndex, std::vector<SpriteAnimationKeyframe>& keyframes, float& currentState) {
	enabled = false; 

	if (keyframes.size() > 0) {
		stepIndex = keyframes.size() - 1;
		currentState = keyframes.back().m_value;
	}
}

bool StateSprites::endSpriteAnimations() {
	//std::cout << "endSpriteAnimations called " << std::endl;
	auto animation{ m_activeSpriteAnimations.begin() };
	for (; animation != m_activeSpriteAnimations.end(); animation++) {
		StepIndices& indicies{ animation->first };
		ActionSpriteAnimation& properties{ animation->second };
		SpriteState& state{ m_spriteRenderData[properties.m_texture] };

		if (properties.m_xposEnabled) {
			endPropertyAnimation(properties.m_xposEnabled, indicies.x, properties.m_xpos, state.m_xpos);
		}

		if (properties.m_yposEnabled) {
			endPropertyAnimation(properties.m_yposEnabled, indicies.y, properties.m_ypos, state.m_ypos);
		}

		if (properties.m_zposEnabled) {
			endPropertyAnimation(properties.m_zposEnabled, indicies.z, properties.m_zpos, state.m_zpos);
		}

		if (properties.m_scaleEnabled) {
			endPropertyAnimation(properties.m_scaleEnabled, indicies.scale, properties.m_scale, state.m_scale);
		}

		if (properties.m_rotationEnabled) {
			endPropertyAnimation(properties.m_rotationEnabled, indicies.rotation, properties.m_rotation, state.m_rotation);
		}

		if (properties.m_opacityEnabled) {
			endPropertyAnimation(properties.m_opacityEnabled, indicies.opacity, properties.m_opacity, state.m_opacity);
		}
	}

	m_activeSpriteAnimations.clear();

	// no return needed
	return true;
}

