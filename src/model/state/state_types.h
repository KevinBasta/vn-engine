#ifndef VN_STATE_TYPES_H
#define VN_STATE_TYPES_H

#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct SpriteState {
	// Current Texture Properties
	bool m_onScreen{ false };
	
	Texture2D* m_texture{ nullptr };
	
	int m_zIndex{ 0 };
	float m_scale{ 1.0f };
	glm::mat4 m_model{ glm::mat4(1.0f) };
};


#endif // VN_STATE_TYPES_H
