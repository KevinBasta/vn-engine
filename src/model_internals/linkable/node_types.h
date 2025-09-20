#ifndef VN_CHAPTER_NODE_TYPES_H
#define VN_CHAPTER_NODE_TYPES_H

#include "id.h"
#include "index.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

enum class NodeActionType {
	BACKGROUND,
	SPRITE,
	TEXT,
	CHOICE,
	RELATION,
};

/**
 * COMMON STRUCTS
 * Identifies textures using the store id and index.
 * Provides hasher for this identifier type.
 */

struct TextureIdentifier {
	id textureStoreId{ 0 };
	index textureIndex{ 0 };

	friend bool operator==(const TextureIdentifier& idOne, const TextureIdentifier& idTwo) {
		return idOne.textureStoreId == idTwo.textureStoreId && idOne.textureIndex == idTwo.textureIndex;
	}
};

struct TextureIdentifierHasher {
	size_t operator()(const TextureIdentifier& textureIdentifier) const {
		return std::hash<id>()(textureIdentifier.textureStoreId) ^
			(std::hash<index>()(textureIdentifier.textureIndex) << 1);
	}
};

/**
 * SPRITE ACTIONS
 * 
 */
enum class SpriteProperty {
	NONE,
	XPOS,
	YPOS,
	ZPOS,
	SCALE,
	ROTATION,
	OPACITY
};

struct ActionSpriteAllProperties {
	TextureIdentifier texture{};

	bool xposEnabled{ false };
	float xpos{ 1.0f };

	bool yposEnabled{ false };
	float ypos{ 1.0f };

	bool zposEnabled{ false };
	float zpos{ 1.0f };

	bool scaleEnabled{ false };
	float scale{ 1.0f };

	bool rotationEnabled{ false };
	float rotation{ 0.0f };

	bool opacityEnabled{ false };
	float opacity{ 1.0f };
};

struct SpriteAnimationKeyframe {
	float transitionSeconds{ 0.0f };
	float value{ 0.0f };
};

struct ActionSpriteAnimation {
	TextureIdentifier texture{};

	bool xposEnabled{ false };
	std::vector<SpriteAnimationKeyframe> xpos{};

	bool yposEnabled{ false };
	std::vector<SpriteAnimationKeyframe> ypos{};

	bool zposEnabled{ false };
	std::vector<SpriteAnimationKeyframe> zpos{};

	bool scaleEnabled{ false };
	std::vector<SpriteAnimationKeyframe> scale{};

	bool rotationEnabled{ false };
	std::vector<SpriteAnimationKeyframe> rotation{};

	bool opacityEnabled{ false };
	std::vector<SpriteAnimationKeyframe> opacity{};
};


/**
 * BACKGROUND ACTIONS
 *
 */
struct ActionBackgroundTexture {
	// Same definiton as ActionSpriteAllProperties but diff
	// name for template overloading ::
	TextureIdentifier texture{};

	bool xposEnabled{ false };
	float xpos{ 1.0f };

	bool yposEnabled{ false };
	float ypos{ 1.0f };

	bool zposEnabled{ false };
	float zpos{ 1.0f };

	bool scaleEnabled{ false };
	float scale{ 1.0f };

	bool rotationEnabled{ false };
	float rotation{ 0.0f };

	bool opacityEnabled{ false };
	float opacity{ 1.0f };
};



/**
 * TEXT ACTIONS
 * 
 */
struct ActionTextLine {
	bool narration{};
	int characterId{};
	std::wstring line{};
};

struct ActionTextOverrides {
	bool renderEnabled{ false };
	bool render{ false };

	bool speakerEnabled{ false };
	std::wstring speaker{};

	bool colorEnabled{ false };
	glm::vec3 color{};
};


/**
 * BOND/RELATIONSHIP ACTIONS
 */

// Modify a relationship
enum class RelationModification {
	SET,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE
};

struct CharacterRelation {
	int characterOneId{};
	int characterTwoId{};

	int relationTypeId{};
};

struct ActionRelationModify {
	CharacterRelation relation{};
	RelationModification modificationType{};
	int modificationValue{};
};




// Relation condition (single)
enum class RelationComparisonOperator {
	LESS_THAN,
	LESS_THAN_OR_EQUAL,
	GREATER_THAN,
	GREATER_THAN_OR_EQUAL,
	EQUAL
};

struct RelationValueComparison {
	CharacterRelation m_relation{};
	int m_valueToCompare{}; // Value is on the right side i.e. (relationValue operator m_valueToCompare)
	RelationComparisonOperator m_comparisonOperator{};
};

struct RelationRelationComparison {
	CharacterRelation m_relationOne{};
	CharacterRelation m_relationTwo{};
	RelationComparisonOperator m_comparisonOperator{};
};


// Relation condition (grouping)
enum class RelationGroupingOperator {
	NONE,
	AND,
	OR
};

struct RelationConditionUnit {
	RelationGroupingOperator m_operator{};
	std::vector<RelationValueComparison> m_relationValueConditions{};
	std::vector<RelationRelationComparison> m_relationRelationConditions{};
	std::vector<RelationConditionUnit> m_conditionUnits{};
};

struct ActionRelationSetNextNode {
	// Set next node based on matching relation conditions
	id m_nodeId{};
	
	// These conditions are ORed
	std::vector<RelationConditionUnit> m_conditions{};
};

struct ActionRelationSetNextChapter {
	// Set next chapter based on matching relation conditions
	id m_chapterId{};

	// These conditions are ORed
	std::vector<RelationConditionUnit> m_conditions{};
};



/** 
 * CHOICE PICKING ATTACHED ACTIONS
 */
using ChoiceIndex = int;

struct ActionChoiceModifyRelation {
	std::vector<std::pair<ChoiceIndex, std::vector<ActionRelationModify>>> relationModifications{};
};

struct ActionChoiceSetNextNode {
	std::vector<std::pair<ChoiceIndex, id>> nodeId{};
};

struct ActionChoiceSetNextChapter {
	std::vector<std::pair<ChoiceIndex, id>> chapterId{};
};



/** 
 * CHOICE INTERFACE
 * 
 */
enum class ChoiceStyle {
	LIST_TEXT_AREA,
	LIST_MID_SCREEN,
};

/*struct ChoiceTileProperties {
	std::wstring m_displayText{};
	Texture2D* m_displayTexture{};
};*/

struct ActionChoice {
	ChoiceStyle style{};

	// TODO: add support for not displaying already visited node
	// The usecase being nodes that branch off and loop back to explore multiple loop paths
	// Would need to figure out how to move forward after all the loop nodes are done
	// maybe something like not origianlly displaying the one non-loop path forward then using it at the end
	// would need the default move-forward behavior to be any node that is not visited. Otherwise the 0th node or error out?
	// but erroring out needs to be recoverable somehow 
	// bool m_doNotDisplayVisited{};
	std::vector<std::wstring> choices{};
};


/** 
 * DIRECT NEXT CHAPTER PICKING
 * 
 */
struct ActionSetNextChapter {
	id chapterId{};
};

#endif // VN_CHAPTER_NODE_TYPES_H
