#ifndef VN_STATE_TYPES_H
#define VN_STATE_TYPES_H

#include "texture.h"
#include "node_types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class StateDelta {
	TEXT,
	BACKGROUND,
	SPRITE
};

struct SpriteState {
	float xpos{ 0.0f };
	float ypos{ 0.0f };
	float zpos{ 0.0f };

	float scale{ 1.0f };
	float rotation{ 0.0f };
	float opacity{ 1.0f };
	// Can add vertex shader, fragment shader to use here too
};

using BackgroundOffsets = SpriteState;

//For typing animation
//enum class TextAction {
//	NONE,
//	TYPING,
//	COMPLETE
//};

struct TextState {
	bool render{ false };

	bool speakerEnabled{ false };
	std::wstring speakerName{};

	bool narrationBackground{ false };
	std::wstring line{};

	glm::vec3 color{};

	//TextAction m_currentState{};
};




#endif // VN_STATE_TYPES_H
