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

	using stepIndex = index;
	
	using spriteRenderMap = std::unordered_map<TextureIdentifier, SpriteState, TextureIdentifierHasher>;
	spriteRenderMap m_spriteRenderData{};
	
	using activeSpriteAnimationsMap = std::list<std::pair<stepIndex, ActionSpriteAnimationGeneric>>;
	activeSpriteAnimationsMap m_activeSpriteAnimations{};
	
public:
	StateSprites(StateSubject* stateSubject) : m_stateSubject{ stateSubject } {}

	void reset() {
		m_spriteRenderData.clear();
		m_activeSpriteAnimations.clear();
	}

public:
	//
	// Node interface
	//
	void handle(const ActionSpriteProperty& action);
	void handle(const ActionSpriteAnimationGeneric& action);
	
public:
	//
	// Auto actions interface
	//
	bool tickAutoActions(float timePassed);
	void endAutoActions() { endSpriteAnimations(); }

private:
	//
	// Auto action helpers
	//
	bool tick(std::pair<stepIndex, ActionSpriteAnimationGeneric>& animation, float timePassed);
	bool tickSpriteAnimations(float timePassed);
	bool endSpriteAnimations();

public:
	//
	// View interface
	//
	const spriteRenderMap& getSpriteRenderData() { return m_spriteRenderData; }

public:
	void load() {}
	void save() {}
};

#endif // VN_STATE_SPRITES_H
