#ifndef VN_CHAPTER_NODE_TYPES_H
#define VN_CHAPTER_NODE_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ChapterNodeSpritePosition {
	
	//int m_zIndex{ 0 };
	//float m_scale{ 1.0f };
	//glm::mat4 m_model{ glm::mat4(1.0f) };
};

struct ChapterNodeSprite {
	int m_characterID{ 0 };
	bool m_onScreen{ false };

	int m_textureIndex{ 0 };
};

#endif // VN_CHAPTER_NODE_TYPES_H
