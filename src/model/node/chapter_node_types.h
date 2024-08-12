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

enum class ChapterNodeActionType {
	BACKGROUND,
	SPRITE,
	TEXT,
	CHOICE,
	RELATION,
};



//
// Sprite Actions
//

enum class SpriteProperty {
	NONE,
	XPOS,
	YPOS,
	ZPOS,
	SCALE,
	OPACITY
};

struct TextureIdentifier {
	id m_textureStoreId{ 0 };
	index m_textureIndex{ 0 };

	friend bool operator==(const TextureIdentifier& idOne, const TextureIdentifier& idTwo) {
		return idOne.m_textureStoreId == idTwo.m_textureStoreId && idOne.m_textureIndex == idTwo.m_textureIndex;
	}
};

struct TextureIdentifierHasher {
	size_t operator()(const TextureIdentifier& textureIdentifier) const {
		return std::hash<int>()(textureIdentifier.m_textureStoreId) ^ 
			   (std::hash<int>()(textureIdentifier.m_textureIndex) << 1);
	}
};

struct ActionSpriteProperty {
	TextureIdentifier m_texture{};
	SpriteProperty m_property{ SpriteProperty::NONE };

	float m_value{ 1.0f };
};

struct ActionSpriteKeyframeGeneric {
	float m_transitionSeconds{ 0.0f };
	float m_value{ 0.0f };
};

struct ActionSpriteAnimationGeneric {
	TextureIdentifier m_texture{};
	SpriteProperty m_stepType{ SpriteProperty::NONE };

	std::vector<ActionSpriteKeyframeGeneric> m_steps{};
};




//
// Text Actions
//
struct ActionTextRender {
	bool m_render{ false };
};

struct ActionTextLine {
	int m_characterID{};
	std::wstring m_line{};
};

struct ActionTextOverrideSpeaker {
	std::wstring m_speakerName{};
};

struct ActionTextOverrideColor {
	glm::vec3 m_textColor{};
};




//
// Background Actions
//
struct ActionBackgroundTexture {
	TextureIdentifier m_texture{};
};




//
// Bonds/Relationship Actions
//

// Modify a relationship
enum class RelationModification {
	SET,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE
};

struct RelationRequested {
	int m_characterOneId{};
	int m_characterTwoId{};

	int m_relationTypeId{};
};

struct ActionRelationModify {
	RelationRequested m_relation{};
	RelationModification m_modificationType{};
	int m_modificationValue{};
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
	RelationRequested m_relation{};
	int m_valueToCompare{}; // Value is on the right side i.e. (relationValue operator m_valueToCompare)
	RelationComparisonOperator m_comparisonOperator{};
};

struct RelationRelationComparison {
	RelationRequested m_relationOne{};
	RelationRequested m_relationTwo{};
	RelationComparisonOperator m_comparisonOperator{};
};


// Relation condition (grouping)
enum class RelationGroupingOperator {
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




//
// Node child picking action
//

using ChoiceIndex = int;

struct ActionChoiceModifyRelation {
	std::unordered_map<ChoiceIndex, std::vector<ActionRelationModify>> m_relationModifications{};
};

struct ActionChoiceSetNextNode {
	std::unordered_map<ChoiceIndex, id> m_nodeId{};
};


//
// Choice Interface
//

enum class ChoiceStyle {
	LIST_TEXT_AREA,
	LIST_MID_SCREEN,
};

/*struct ChoiceTileProperties {
	std::wstring m_displayText{};
	Texture2D* m_displayTexture{};
};*/

struct ActionChoice {
	ChoiceStyle m_style{};

	// TODO: add support for not displaying already visited node
	// The usecase being nodes that branch off and loop back to explore multiple loop paths
	// Would need to figure out how to move forward after all the loop nodes are done
	// maybe something like not origianlly displaying the one non-loop path forward then using it at the end
	// would need the default move-forward behavior to be any node that is not visited. Otherwise the 0th node or error out?
	// but erroring out needs to be recoverable somehow 
	// bool m_doNotDisplayVisited{};
	std::vector<std::wstring> m_choices{};
};


#endif // VN_CHAPTER_NODE_TYPES_H
