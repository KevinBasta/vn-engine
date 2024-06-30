
#include "state_subject.h"

//typedef int ID;
//typedef std::unordered_map<ID, SpriteState> spriteRenderMap;

void StateSubject::initCharacterData() {
	const ModelSubject::characterMap& characterMap = m_model->getCharacters();

	for (auto iter = characterMap.begin(); iter != characterMap.end(); iter++) {
		m_spriteRenderData[iter->first] = SpriteState{};
	}
}

void StateSubject::handle(ActionSpriteTexture& action) {
	try {
		SpriteState& state = m_spriteRenderData.at(action.m_characterID);

		// TODO: further null and err checks needed
		state.m_texture = m_model->getCharacterByID(action.m_characterID)->getTexture(action.m_textureIndex);
	}
	catch (std::out_of_range) {
		std::cerr << "ActionSpriteTexture::out_of_range" << std::endl;
	}

	m_stateDelta.push_back(StateDelta::SPRITE);
}

void StateSubject::handle(ActionSpriteOpacity& action) {
	try {
		SpriteState& state = m_spriteRenderData.at(action.m_characterID);
		state.m_position.m_opacity = action.m_opacity;
	}
	catch (std::out_of_range) {
		std::cerr << "ActionSpriteOpacity::out_of_range" << std::endl;
	}

	m_stateDelta.push_back(StateDelta::SPRITE);
}

void StateSubject::handle(ActionSpritePosition& action) {
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

	m_stateDelta.push_back(StateDelta::SPRITE);
}

void StateSubject::handle(ActionSpriteAnimationGeneric& action)
{
	std::cout << "auto action handling" << std::endl;

	setAutoAction();
	m_activeSpriteAnimations.push_back(std::pair{ 0, action });
}


// return true if animation is done, false otherwise
bool StateSubject::tick(float& characterValue, std::pair<stepIndex, ActionSpriteAnimationGeneric>& animation, float timePassed) {
	if (animation.first > animation.second.m_steps.size() - 1) {
		return true;
	}

	auto& animationStepIndex{ animation.first };
	auto& currentAction{ animation.second.m_steps[animation.first] };

	float fractionOfTimePassed = currentAction.m_transitionSeconds / timePassed;

	float currentX = characterValue;
	float goalX = currentAction.m_value;

	float delta = (std::max(currentX, goalX) - std::min(currentX, goalX)) / fractionOfTimePassed;

	if (currentX < goalX) {
		characterValue += delta;
	}
	else {
		characterValue -= delta;
	}

	currentAction.m_transitionSeconds -= timePassed;

	if (std::abs(characterValue - currentAction.m_value) <= 5.0f && currentAction.m_transitionSeconds < 0.001f) {
		
		characterValue = currentAction.m_value;
		
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
bool StateSubject::tickSpriteAnimations(float timePassed) {
	bool animationsActive{ false };

	auto animation{ m_activeSpriteAnimations.begin() };
	for (; animation != m_activeSpriteAnimations.end(); animation++) {
		auto& obj{ *animation };
		bool done{ false };
		auto& characterState{ m_spriteRenderData[obj.second.m_characterID] };


		switch (obj.second.m_stepType) {
			case (SpriteProperty::XPOS):
				done = tick(characterState.m_position.m_xCoord, obj, timePassed);
				break;
			case (SpriteProperty::YPOS):
				done = tick(characterState.m_position.m_yCoord, obj, timePassed);
				break;
			default:
				break;
		}

		if (done) {
			// Iterator invalidated when item erased, so go to next item
			// TODO: THIS MIGHT HAVE A BUG, CHECK RETURN OF ERASE
			animation = m_activeSpriteAnimations.erase(animation);
			if (animation == m_activeSpriteAnimations.end()) {
				break;
			}
		}
		else {
			animationsActive = true;
		}
	}

	return animationsActive;
}

bool StateSubject::endSpriteAnimations() {
	std::cout << "endSpriteAnimations called " << std::endl;
	auto animation{ m_activeSpriteAnimations.begin() };
	for (; animation != m_activeSpriteAnimations.end(); animation++) {
		auto& obj{ *animation };
		auto& characterPos{ m_spriteRenderData[obj.second.m_characterID].m_position };
		float& endValue{ obj.second.m_steps[obj.second.m_steps.size() - 1].m_value };

		switch (obj.second.m_stepType) {
			case (SpriteProperty::XPOS):
			{
				std::cout << "XPOS ANIMATION TERMINATION!!!!!!!!!!!!!!!" << std::endl;
				characterPos.m_xCoord = endValue;
				break;
			}
			case (SpriteProperty::YPOS):
			{
				std::cout << "YPOS ANIMATION TERMINATION!!!!!!!!!!!!!!!" << std::endl;
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


void StateSubject::tickAutoActions(float timePassed) {
	// proof of concept for character animation
	//std::cout << "tick auto actions" << std::endl;
	//std::cout << timePassed << std::endl;
	bool anyActive{ false };

	anyActive |= tickSpriteAnimations(timePassed);
	
	//m_spriteRenderData[1].m_position.m_xCoord += (std::max(currentX, goalX) - std::min(currentX, goalX)) / fractionOfTimePassed;

	//m_spriteAnimationGoal[1].second.m_steps[m_spriteAnimationGoal[1].first].m_transitionSeconds -= timePassed;

	if (anyActive == false) {
		clearAutoAction();
		// increase the spriteanimalgoal.first
		// if that step does not exist, delete this auto action.
		// check if any auto actions ran this tick. If none, then clear the auto action.
	}
}


void StateSubject::endAutoActions() {
	// Go to end of all auto actions
	endSpriteAnimations();

}

//struct ActionSpriteKeyframe {
//	float m_transitionSeconds{ 0.0f };
//
//	float m_xCoord{ 0.0f };
//	float m_yCoord{ 0.0f };
//	float m_zCoord{ 0.0f };
//	float m_scale{ 1.0f };
//	float m_opacity{ 1.0f };
//};
//
//struct SpritePosition {
//	float m_xCoord{ 0.0f };
//	float m_yCoord{ 0.0f };
//	float m_zCoord{ 0.0f };
//
//	float m_scale{ 1.0f };
//	float m_opacity{ 1.0f };
//};
//
//struct SpriteState {
//	Texture2D* m_texture{ nullptr };
//
//	SpritePosition m_position{};
//};