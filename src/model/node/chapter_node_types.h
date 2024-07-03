#ifndef VN_CHAPTER_NODE_TYPES_H
#define VN_CHAPTER_NODE_TYPES_H

#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <list>

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
struct ActionBackgroundTexture{
	int backgroundIndex{};
};




//
// Node child picking action
//


enum class ChoiceStyle {
	LIST_TEXT_AREA,
	LIST_MID_SCREEN,
};

struct ChoiceProperties {
	int m_nodeID{};

	std::wstring m_displayText{};
};

struct ActionPickChild {
	ChoiceStyle m_style{};

	std::list<ChoiceProperties> m_choices{};
};


#endif // VN_CHAPTER_NODE_TYPES_H
