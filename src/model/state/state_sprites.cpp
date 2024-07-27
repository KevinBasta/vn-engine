
#include "state_subject.h"
#include "state_types.h"
#include "model_subject.h"
#include "state_sprites.h"
#include "chapter_node_types.h"

void StateSprites::initCharacterData() {
	const ModelSubject::characterMap& characterMap = ModelSubject::getCharacters();

	for (auto iter = characterMap.begin(); iter != characterMap.end(); iter++) {
		m_spriteRenderData[iter->first] = SpriteState{};
	}
}

void StateSprites::handle(ActionSpriteTexture& action) {
	try {
		SpriteState& state = m_spriteRenderData.at(action.m_characterID);

		// TODO: further null and err checks needed
		state.m_texture = TextureManager::getTexture(ModelSubject::getCharacterByID(action.m_characterID)->getTextureIdByIndex(action.m_textureIndex));
	}
	catch (std::out_of_range) {
		std::cerr << "ActionSpriteTexture::out_of_range" << std::endl;
	}

	//m_stateDelta.push_back(StateDelta::SPRITE);
}

void StateSprites::handle(ActionSpriteOpacity& action) {
	try {
		SpriteState& state = m_spriteRenderData.at(action.m_characterID);
		state.m_position.m_opacity = action.m_opacity;
	}
	catch (std::out_of_range) {
		std::cerr << "ActionSpriteOpacity::out_of_range" << std::endl;
	}

	//m_stateDelta.push_back(StateDelta::SPRITE);
}

void StateSprites::handle(ActionSpritePosition& action) {
	try {
		SpriteState& state = m_spriteRenderData.at(action.m_characterID);

		state.m_position.m_xCoord = action.m_xCoord;
		state.m_position.m_yCoord = action.m_yCoord;
		state.m_position.m_zCoord = action.m_zCoord;
		state.m_position.m_scale = action.m_scale;
	}
	catch (std::out_of_range) {
		std::cerr << "ActionSpritePosition::out_of_range" << std::endl;
	}

	//m_stateDelta.push_back(StateDelta::SPRITE);
}

void StateSprites::handle(ActionSpriteAnimationGeneric& action)
{
	std::cout << "auto action handling" << std::endl;
	// TODO: null check
	m_stateSubject->setAutoAction();
	m_activeSpriteAnimations.push_back(std::pair{ 0, action });
}


// return true if animation is done, false otherwise
bool StateSprites::tick(std::pair<stepIndex, ActionSpriteAnimationGeneric>& animation, float timePassed) {
	if (animation.first > animation.second.m_steps.size() - 1) {
		return true;
	}

	auto& animationStepIndex{ animation.first };
	auto& currentAction{ animation.second.m_steps[animation.first] };

	auto& characterState{ m_spriteRenderData[animation.second.m_characterID] };
	float* currentValue{ nullptr };
	float goalValue = (currentAction.m_value);

	switch (animation.second.m_stepType) {
	case (SpriteProperty::XPOS):
		currentValue = &(characterState.m_position.m_xCoord);
		break;
	case (SpriteProperty::YPOS):
		currentValue = &(characterState.m_position.m_yCoord);
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
		auto& characterState{ m_spriteRenderData[obj.second.m_characterID] };

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
		auto& characterPos{ m_spriteRenderData[obj.second.m_characterID].m_position };
		float& endValue{ obj.second.m_steps[obj.second.m_steps.size() - 1].m_value };

		switch (obj.second.m_stepType) {
		case (SpriteProperty::XPOS):
		{
			std::cout << "XPOS END SPRITE ANIMATION" << std::endl;
			characterPos.m_xCoord = endValue;
			break;
		}
		case (SpriteProperty::YPOS):
		{
			std::cout << "YPOS END SPRITE ANIMATION" << std::endl;
			characterPos.m_yCoord = endValue;
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