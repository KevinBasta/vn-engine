#ifndef VN_CHAPTER_NODE_TYPES_H
#define VN_CHAPTER_NODE_TYPES_H

#include "id.h"
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

struct ActionSpriteOpacity {
	int m_characterID{ 0 };

	float m_opacity{ 0.0f };
};

struct ActionSpriteTexture {
	int m_characterID{ 0 };

	int m_textureIndex{ 0 };
};

struct ActionSpritePosition {
	int m_characterID{ 0 };
	
	float m_xCoord	{ 0.0f };
	float m_yCoord	{ 0.0f };
	float m_zCoord  { 0.0f };
	float m_scale	{ 1.0f };
};



// Have separate ones for different values? 
// like ActionSpriteAnimationX for moving the x axis?
// allows for decoupling of the m_transitionSeconds
// but increased objects significantly
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
//struct ActionSpriteAnimation {
//	int m_characterID{ 0 };
//
//	std::vector<ActionSpriteKeyframe> m_steps{};
//};

enum class SpriteProperty {
	NONE,
	XPOS,
	YPOS,
	ZPOS,
	SCALE,
	OPACITY
};

struct ActionSpriteKeyframeGeneric {
	float m_transitionSeconds{ 0.0f };
	float m_value{ 0.0f };
};

struct ActionSpriteAnimationGeneric {
	int m_characterID{ 0 };
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
	int backgroundIndex{};
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

struct ActionRelationCondition {
	RelationRequested m_relationType{};
	
	RelationComparisonOperator m_comparisonOperator{};
	// Value is on the right side i.e. (relationValue operator m_valueToCompare)
	int m_valueToCompare{};
};


// Relation condition (grouping)
enum class RelationGroupingOperator {
	NONE,
	AND,
	OR
};

struct ActionRelationConditionUnit {
	std::vector<ActionRelationCondition> m_conditions{};
	RelationGroupingOperator m_operator{};
};

struct ActionRelationSetNextNode {
	// Set next node based on matching relation conditions
	id m_nodeID{};
	
	// These conditions are ORed
	std::vector<ActionRelationConditionUnit> m_conditions{};
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
