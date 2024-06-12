#ifndef VN_CHAPTER_NODE_TYPES_H
#define VN_CHAPTER_NODE_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


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
	int m_zIndex{ 0 };
	float m_scale{ 1.0f };
	glm::mat4 m_model{ glm::mat4(1.0f) };
};


//
// Text Actions
//
struct ChapterNodeText {
	int characterID{};
	std::string line{};

	bool overrideSpeakerName{};
	std::string speakerName{};
};


//
// Background Actions
//
struct ActionBackgroundTexture{
	int backgroundIndex{};
};

#endif // VN_CHAPTER_NODE_TYPES_H
