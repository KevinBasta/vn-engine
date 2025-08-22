
#include "state_subject.h"
#include "state_types.h"
#include "model_subject.h"
#include "model_runtime_interface.h"
#include "state_sprites.h"
#include "node_types.h"


//
// SPRITE PROPERTIES
//
//

void StateSprites::handle(const ActionSpriteAllProperties& action) {
	if (m_spriteRenderData.find(action.texture) == m_spriteRenderData.end()) {
		// TODO: handle error/exceptions

		// TODO: should propergate error from load texture!
		ModelRuntimeInterface::loadTexture(action.texture);
	}

	if (action.xposEnabled) {
		m_spriteRenderData[action.texture].xpos = action.xpos;
	}

	if (action.yposEnabled) {
		m_spriteRenderData[action.texture].ypos = action.ypos;
	}

	if (action.zposEnabled) {
		m_spriteRenderData[action.texture].zpos = action.zpos;
	}

	if (action.scaleEnabled) {
		m_spriteRenderData[action.texture].scale = action.scale;
	}
	
	if (action.rotationEnabled) {
		m_spriteRenderData[action.texture].rotation = action.rotation;
	}

	if (action.opacityEnabled) {
		m_spriteRenderData[action.texture].opacity = action.opacity;
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
			properties.xposEnabled || properties.yposEnabled || properties.zposEnabled || 
			properties.scaleEnabled || properties.rotationEnabled || properties.opacityEnabled 
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
	SpriteState& state{ m_spriteRenderData[properties.texture] };

	if (properties.xposEnabled) {
		tickPropertyAnimation(properties.xposEnabled, timePassed, indicies.xpos, properties.xpos, state.xpos);
		active = true;
	}

	if (properties.yposEnabled) {
		tickPropertyAnimation(properties.yposEnabled, timePassed, indicies.ypos, properties.ypos, state.ypos);
		active = true;
	}

	if (properties.zposEnabled) {
		tickPropertyAnimation(properties.zposEnabled, timePassed, indicies.zpos, properties.zpos, state.zpos);
		active = true;
	}

	if (properties.scaleEnabled) {
		tickPropertyAnimation(properties.scaleEnabled, timePassed, indicies.scale, properties.scale, state.scale);
		active = true;
	}

	if (properties.rotationEnabled) {
		tickPropertyAnimation(properties.rotationEnabled, timePassed, indicies.rotation, properties.rotation, state.rotation);
		active = true;
	}

	if (properties.opacityEnabled) {
		tickPropertyAnimation(properties.opacityEnabled, timePassed, indicies.opacity, properties.opacity, state.opacity);
		active = true;
	}

	return active;
}

bool StateSprites::tickPropertyAnimation(bool& enabled, float timePassed, index& stepIndex, std::vector<SpriteAnimationKeyframe>& keyframes, float& currentState) {
	if (stepIndex > keyframes.size() - 1 || keyframes.size() == 0 || enabled == false) {
		enabled = false;
		return false;
	}

	if (keyframes[stepIndex].transitionSeconds == 0) {
		if (stepIndex + 1 > keyframes.size() - 1) {
			enabled = false;
			return false;
		}
		else {
			stepIndex++;
		}

		return true;
	}

	// TODO: error when engine modifies this
	auto& currentAction{ keyframes[stepIndex] };
	float goalValue = (currentAction.value);

	float fractionOfTimePassed = currentAction.transitionSeconds / timePassed;

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

	currentAction.transitionSeconds -= timePassed;

	// If the character value state is close enough to the goal value and the transition seconds left is low
	// Then set the character value state to the goal state and set the goal to the next step or end the animation
	if (std::abs(currentState - currentAction.value) <= 5.0f && currentAction.transitionSeconds < 0.1f) {
		currentState = currentAction.value;

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
		currentState = keyframes.back().value;
	}
}

bool StateSprites::endSpriteAnimations() {
	//std::cout << "endSpriteAnimations called " << std::endl;
	auto animation{ m_activeSpriteAnimations.begin() };
	for (; animation != m_activeSpriteAnimations.end(); animation++) {
		StepIndices& indicies{ animation->first };
		ActionSpriteAnimation& properties{ animation->second };
		SpriteState& state{ m_spriteRenderData[properties.texture] };

		if (properties.xposEnabled) {
			endPropertyAnimation(properties.xposEnabled, indicies.xpos, properties.xpos, state.xpos);
		}

		if (properties.yposEnabled) {
			endPropertyAnimation(properties.yposEnabled, indicies.ypos, properties.ypos, state.ypos);
		}

		if (properties.zposEnabled) {
			endPropertyAnimation(properties.zposEnabled, indicies.zpos, properties.zpos, state.zpos);
		}

		if (properties.scaleEnabled) {
			endPropertyAnimation(properties.scaleEnabled, indicies.scale, properties.scale, state.scale);
		}

		if (properties.rotationEnabled) {
			endPropertyAnimation(properties.rotationEnabled, indicies.rotation, properties.rotation, state.rotation);
		}

		if (properties.opacityEnabled) {
			endPropertyAnimation(properties.opacityEnabled, indicies.opacity, properties.opacity, state.opacity);
		}
	}

	m_activeSpriteAnimations.clear();

	// no return needed
	return true;
}

