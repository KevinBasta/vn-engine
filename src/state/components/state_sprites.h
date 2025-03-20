#ifndef VN_STATE_SPRITES_H
#define VN_STATE_SPRITES_H

#include "id.h"
#include "state_types.h"
#include "chapter_node_types.h"

#include <unordered_map>
#include <list>

class StateSubject;


// TODO: disallow negative seconds
class StateSprites {

//
// General and constructor
//

private:
	StateSubject* m_stateSubject{ nullptr };

public:
	StateSprites(StateSubject* stateSubject) :
		m_stateSubject{ stateSubject }
	{
	}

	void reset() {
		m_spriteRenderData.clear();
		m_activeSpriteAnimations.clear();
	}



//
// SPRITE PROPERIES
//

private:
	using spriteRenderMap = std::unordered_map<TextureIdentifier, SpriteState, TextureIdentifierHasher>;
	spriteRenderMap m_spriteRenderData{};

public:
	// Node interface
	void handle(const ActionSpriteAllProperties& action);

	// View interface
	const spriteRenderMap& getSpriteRenderData() { return m_spriteRenderData; }



//
// SPRITE ANIMATIONS (auto actions)
//

private:
	struct StepIndices { index xpos; index ypos; index zpos; index scale; index rotation; index opacity; };
	using activeSpriteAnimationsMap = std::list<std::pair<StepIndices, ActionSpriteAnimation>>;
	activeSpriteAnimationsMap m_activeSpriteAnimations{};

public:
	// Node interface
	void handle(const ActionSpriteAnimation& action);

	// Auto actions interface
	bool tickAutoActions(float timePassed);
	void endAutoActions() { endSpriteAnimations(); }

private:
	// Internal auto actions helpers
	bool tick(std::pair<StepIndices, ActionSpriteAnimation>& animation, float timePassed);
	bool tickPropertyAnimation(bool& enabled, float timePassed, index& stepIndex, std::vector<SpriteAnimationKeyframe>& keyframes, float& currentState);
	bool tickSpriteAnimations(float timePassed);
	bool endSpriteAnimations();
	void endPropertyAnimation(bool& enabled, index& stepIndex, std::vector<SpriteAnimationKeyframe>& keyframes, float& currentState);

public:
	void load() {}
	void save() {}
};

#endif // VN_STATE_SPRITES_H
