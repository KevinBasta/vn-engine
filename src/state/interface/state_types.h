#ifndef VN_STATE_TYPES_H
#define VN_STATE_TYPES_H

#include "texture.h"
#include "chapter_node_types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class StateDelta {
	TEXT,
	BACKGROUND,
	SPRITE
};

struct SpriteState {
	float m_xpos{ 0.0f };
	float m_ypos{ 0.0f };
	float m_zpos{ 0.0f };

	float m_scale{ 1.0f };
	float m_rotation{ 0.0f };
	float m_opacity{ 1.0f };
	// Can add vertex shader, fragment shader to use here too
};

//For typing animation
//enum class TextAction {
//	NONE,
//	TYPING,
//	COMPLETE
//};

struct TextState {
	bool m_render{ false };
	//TextAction m_currentState{};

	std::wstring m_speakerName{};
	std::wstring m_line{};
	glm::vec3 m_color{};
};




#endif // VN_STATE_TYPES_H
