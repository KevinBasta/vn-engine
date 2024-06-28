
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

void StateSubject::handle(int characterID, ActionSpriteAnimation& action)
{
	std::cout << "substep handling" << std::endl;
	std::cout << "substep handling" << std::endl;
	std::cout << "substep handling" << std::endl;
	std::cout << "substep handling" << std::endl;
	std::cout << "substep handling" << std::endl;
	std::cout << "substep handling" << std::endl;
	setAutoAction();
	m_spriteAnimationGoal[characterID] = std::pair{ 0, action };

}

void StateSubject::tickAutoActions(float timePassed) {
	// proof of concept for character animation
	std::cout << "tick auto actions" << std::endl;
	std::cout << timePassed << std::endl;
	float fractionOfTimePassed = m_spriteAnimationGoal[1].second.m_steps[m_spriteAnimationGoal[1].first].m_transitionSeconds / timePassed;

	float currentX = m_spriteRenderData[1].m_position.m_xCoord;
	float goalX = m_spriteAnimationGoal[1].second.m_steps[m_spriteAnimationGoal[1].first].m_xCoord;

	std::cout << "x add: " << (std::max(currentX, goalX) - std::min(currentX, goalX)) / fractionOfTimePassed << std::endl;

	m_spriteRenderData[1].m_position.m_xCoord += (std::max(currentX, goalX) - std::min(currentX, goalX)) / fractionOfTimePassed;

	m_spriteAnimationGoal[1].second.m_steps[m_spriteAnimationGoal[1].first].m_transitionSeconds -= timePassed;

	if (m_spriteRenderData[1].m_position.m_xCoord >= m_spriteAnimationGoal[1].second.m_steps[m_spriteAnimationGoal[1].first].m_xCoord) {
		clearAutoAction();
	}
}


void StateSubject::seekAutoActions() {
	// Go to end of all auto actions

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