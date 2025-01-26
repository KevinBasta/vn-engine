
#include "state_subject.h"
#include "state_types.h"
#include "model_subject.h"
#include "model_runtime_interface.h"
#include "state_sprites.h"
#include "chapter_node_types.h"

void StateSprites::handle(const ActionSpriteProperty& action) {
	if (m_spriteRenderData.find(action.m_texture) == m_spriteRenderData.end()) {
		// TODO: handle error/exceptions
		ModelRuntimeInterface::loadTexture(action.m_texture);		
	}

	switch (action.m_property)
	{
	case SpriteProperty::XPOS:
	{
		m_spriteRenderData[action.m_texture].m_xpos = action.m_value;
		break;
	}
	case SpriteProperty::YPOS:
	{
		m_spriteRenderData[action.m_texture].m_ypos = action.m_value;
		break;
	}
	case SpriteProperty::ZPOS:
	{
		m_spriteRenderData[action.m_texture].m_zpos = action.m_value;
		break;
	}
	case SpriteProperty::SCALE:
	{
		m_spriteRenderData[action.m_texture].m_scale = action.m_value;
		break;
	}
	case SpriteProperty::OPACITY:
	{
		m_spriteRenderData[action.m_texture].m_opacity = action.m_value;
		break;
	}
	default:
		break;
	}
}

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
	
	if (action.m_opacityEnabled) {
		m_spriteRenderData[action.m_texture].m_opacity = action.m_opacity;
	}
}

void StateSprites::handle(const ActionSpriteAnimationGeneric& action)
{
	std::cout << "auto action handling" << std::endl;
	// TODO: null check
	m_stateSubject->setAutoAction();
	m_activeSpriteAnimations.push_back(std::pair{ 0, action });
}

// TODO: this is a little jerkey at the end of scale animations. perhaps allow time curve options.
// return true if animation is done, false otherwise
bool StateSprites::tick(std::pair<stepIndex, ActionSpriteAnimationGeneric>& animation, float timePassed) {
	if (animation.first > animation.second.m_steps.size() - 1) {
		return true;
	}

	auto& animationStepIndex{ animation.first };
	auto& currentAction{ animation.second.m_steps[animation.first] };

	auto& characterState{ m_spriteRenderData[animation.second.m_texture] };
	float* currentValue{ nullptr };
	float goalValue = (currentAction.m_value);

	switch (animation.second.m_stepType) {
	case (SpriteProperty::XPOS):
		currentValue = &(characterState.m_xpos);
		break;
	case (SpriteProperty::YPOS):
		currentValue = &(characterState.m_ypos);
		break;
	case (SpriteProperty::ZPOS):
		currentValue = &(characterState.m_zpos);
		break;
	case (SpriteProperty::SCALE):
		currentValue = &(characterState.m_scale);
		break;
	case (SpriteProperty::OPACITY):
		currentValue = &(characterState.m_opacity);
		break;
	default:
		break;
	}

	// End animation if the current value is a nullptr
	if (currentValue == nullptr) {
		return true;
	}

	float fractionOfTimePassed = currentAction.m_transitionSeconds / timePassed;

	float delta = (std::max(*currentValue, goalValue) - std::min(*currentValue, goalValue)) / fractionOfTimePassed;

	if (*currentValue < goalValue) {
		if (*currentValue + delta > goalValue) {
			*currentValue = goalValue;
		}
		else {
			*currentValue += delta;
		}
	}
	else {
		if (*currentValue - delta < goalValue) {
			*currentValue = goalValue;
		}
		else {
			*currentValue -= delta;
		}
	}

	currentAction.m_transitionSeconds -= timePassed;

	// If the character value state is close enough to the goal value and the transition seconds left is low
	// Then set the character value state to teh goal state and set the goal to the next step or end the animation
	if (std::abs(*currentValue - currentAction.m_value) <= 5.0f && currentAction.m_transitionSeconds < 0.1f) {

		*currentValue = currentAction.m_value;

		if (animationStepIndex + 1 > animation.second.m_steps.size() - 1) {
			return true;
		}
		else {
			animationStepIndex++;
		}
	}

	return false;
}

// return true if animation is done, false otherwise
bool StateSprites::tickSpriteAnimations(float timePassed) {
	bool animationsActive{ false };

	auto animation{ m_activeSpriteAnimations.begin() };
	for (; animation != m_activeSpriteAnimations.end();) {
		auto& obj{ *animation };
		auto& characterState{ m_spriteRenderData[obj.second.m_texture] };

		bool done{ tick(obj, timePassed) };

		if (done) {
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

bool StateSprites::endSpriteAnimations() {
	//std::cout << "endSpriteAnimations called " << std::endl;
	auto animation{ m_activeSpriteAnimations.begin() };
	for (; animation != m_activeSpriteAnimations.end(); animation++) {
		auto& obj{ *animation };
		auto& characterPos{ m_spriteRenderData[obj.second.m_texture] };
		float& endValue{ obj.second.m_steps[obj.second.m_steps.size() - 1].m_value };

		switch (obj.second.m_stepType) {
		case (SpriteProperty::XPOS):
		{
			std::cout << "XPOS END SPRITE ANIMATION" << std::endl;
			characterPos.m_xpos = endValue;
			break;
		}
		case (SpriteProperty::YPOS):
		{
			std::cout << "YPOS END SPRITE ANIMATION" << std::endl;
			characterPos.m_ypos = endValue;
			break;
		}
		case (SpriteProperty::ZPOS):
		{
			std::cout << "ZPOS END SPRITE ANIMATION" << std::endl;
			characterPos.m_zpos = endValue;
			break;
		}
		case (SpriteProperty::SCALE):
		{
			std::cout << "SCALE END SPRITE ANIMATION" << std::endl;
			characterPos.m_scale = endValue;
			break;
		}
		case (SpriteProperty::OPACITY):
		{
			std::cout << "OPACITY END SPRITE ANIMATION" << std::endl;
			characterPos.m_opacity = endValue;
			break;
		}
		default:
		{
			break;
		}
		}
	}

	m_activeSpriteAnimations.clear();

	// no return needed
	return true;
}

bool StateSprites::tickAutoActions(float timePassed) {
	bool anyActive{ false };

	anyActive |= tickSpriteAnimations(timePassed);

	return anyActive;
}