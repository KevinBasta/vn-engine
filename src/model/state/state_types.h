#ifndef VN_STATE_TYPES_H
#define VN_STATE_TYPES_H

#include "texture.h"
#include "chapter_node_types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct SpritePosition {
	float m_xCoord{ 0.0f };
	float m_yCoord{ 0.0f };
	float m_zCoord{ 0.0f };

	float m_scale{ 1.0f };
	float m_opacity{ 1.0f };
};

struct SpriteState {
	Texture2D* m_texture{ nullptr };
	
	SpritePosition m_position{};
};

enum class TextAction {
	EMPTY,
	TYPING,
	COMPLETE
};

struct TextState {
	TextAction m_currentState{};
	std::wstring_view m_speakerName{};
	std::wstring_view m_line{};
	glm::vec3 m_color{};
};




#endif // VN_STATE_TYPES_H
