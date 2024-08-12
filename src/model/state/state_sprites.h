#ifndef VN_STATE_SPRITES_H
#define VN_STATE_SPRITES_H

#include "id.h"
#include "state_types.h"
#include "chapter_node_types.h"

#include <unordered_map>
#include <list>

class StateSubject;

class StateSprites {
private:
	StateSubject* m_stateSubject{ nullptr };

public:
	StateSprites(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}

	void reset() {
		m_spriteRenderData.clear();
		m_activeSpriteAnimations.clear();
	}

	using stepIndex = int;
	
	// TODO: allow multiple sprites for one character
	// TODO: rethink sprite system. perhaps a central sprite manager
	using spriteRenderMap = std::unordered_map<TextureIdentifier, SpriteState, TextureIdentifierHasher>;
	using activeSpriteAnimationsMap = std::list<std::pair<stepIndex, ActionSpriteAnimationGeneric>>;

	spriteRenderMap m_spriteRenderData{};
	activeSpriteAnimationsMap m_activeSpriteAnimations{};
	spriteRenderMap& getSpriteRenderData() { return m_spriteRenderData; }

	void handle(ActionSpriteProperty& action);
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
