
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

void StateSubject::handle(ActionSpriteAnimation action)
{
	std::cout << "substep handling" << std::endl;

	setAutoAction();
	m_activeSpriteAnimations.push_back(std::pair{ 0, action });
}

// return true if animation is done, false otherwise
bool StateSubject::tick(std::pair<stepIndex, ActionSpriteAnimation>& action, float timePassed) {
	auto& currentAction { action.second.m_steps[action.first] };
	auto& characterState{ m_spriteRenderData[action.second.m_characterID] };

	float fractionOfTimePassed = currentAction.m_transitionSeconds / timePassed;

	float currentX = characterState.m_position.m_xCoord;
	float goalX = currentAction.m_xCoord;

	float delta = (std::max(currentX, goalX) - std::min(currentX, goalX)) / fractionOfTimePassed;
	//std::cout << "delta: " << delta << std::endl;

	if (currentX < goalX) {
		characterState.m_position.m_xCoord += delta;
	}
	else {
		characterState.m_position.m_xCoord -= delta;
	}

	currentAction.m_transitionSeconds -= timePassed;

	std::cout << "abs: " << std::abs(characterState.m_position.m_xCoord - currentAction.m_xCoord) << std::endl;
	if (std::abs(characterState.m_position.m_xCoord - currentAction.m_xCoord) <= 5.0f) {
		characterState.m_position.m_xCoord = currentAction.m_xCoord;
		if (action.second.m_steps.size() - 1 < action.first + 1) {
			return true;
		}
		else {
			action.first++;
		}
	}

	return false;
}

void StateSubject::tickAutoActions(float timePassed) {
	// proof of concept for character animation
	//std::cout << "tick auto actions" << std::endl;
	//std::cout << timePassed << std::endl;
	bool anyActive{ false };

	for (auto animation{ m_activeSpriteAnimations.begin() }; animation != m_activeSpriteAnimations.end(); animation++) {
		bool done = tick(*animation, timePassed);
		
		if (done) {
			// Iterator invalidated when item erased, so go to next item
			animation = m_activeSpriteAnimations.erase(animation);
			if (animation == m_activeSpriteAnimations.end()) {
				break;
			}
		}
		else {
			anyActive = true;
		}
	}
	
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
	for (auto animation{ m_activeSpriteAnimations.begin() }; animation != m_activeSpriteAnimations.end(); animation++) {
		auto action = *animation;
		
		auto& characterState{ m_spriteRenderData[action.second.m_characterID] };

		for (int i{ action.first }; i < action.second.m_steps.size(); i++) {
			auto& currentAction{ action.second.m_steps[i] };
			characterState.m_position.m_xCoord = currentAction.m_xCoord;
			std::cout << characterState.m_position.m_xCoord << std::endl;;
		}

		animation = m_activeSpriteAnimations.erase(animation);
		if (animation == m_activeSpriteAnimations.end()) {
			break;
		}
	}
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