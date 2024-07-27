#ifndef VN_STATE_SPRITES_H
#define VN_STATE_SPRITES_H

#include "id.h"
#include "state_types.h"
#include "chapter_node_types.h"

#include <unordered_map>
#include <list>

class StateSubject;

class StateSprites {
public:
	StateSubject* m_stateSubject{ nullptr };
	StateSprites(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}

	using characterId = id;
	using stepIndex = int;
	
	// TODO: allow multiple sprites for one character
	// TODO: rethink sprite system. perhaps a central sprite manager
	using spriteRenderMap = std::unordered_map<characterId, SpriteState>;
	using activeSpriteAnimationsMap = std::list<std::pair<stepIndex, ActionSpriteAnimationGeneric>>;

	spriteRenderMap m_spriteRenderData{};
	activeSpriteAnimationsMap m_activeSpriteAnimations{};
	spriteRenderMap& getSpriteRenderData() { return m_spriteRenderData; }
	void initCharacterData();

	void handle(ActionSpriteTexture& action);
	void handle(ActionSpriteOpacity& action);
	void handle(ActionSpritePosition& action);
	void handle(ActionSpriteAnimationGeneric& action);
	bool tick(std::pair<stepIndex, ActionSpriteAnimationGeneric>& animation, float timePassed);
	bool tickSpriteAnimations(float timePassed);
	bool endSpriteAnimations();

	bool tickAutoActions(float timePassed) {
		bool anyActive{ false };

		anyActive |= tickSpriteAnimations(timePassed);

		return anyActive;
	}

	void endAutoActions() {
		endSpriteAnimations();
	}
};

#endif // VN_STATE_SPRITES_H
