#ifndef VN_SPRITE_LAYER_H
#define VN_SPRITE_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"

#define TEMP_VERTEX_PATH	VN_BASE_PATH"/src/view/glsl/vertex_texture.glsl"
#define TEMP_FRAGMENT_PATH	VN_BASE_PATH"/src/view/glsl/fragment_texture.glsl"

class SpriteLayer {
private:
	VnWindow* m_window{ nullptr };
	Shader m_defaultShader;
	StateSubject* m_stateSubject{ nullptr };

	void drawSprite(const FrameDimensions& frame, const TextureIdentifier& textureIdentifier, const SpriteState& spriteState);

public:
	SpriteLayer(VnWindow* window, StateSubject* stateSubject):
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH }
	{
	}

	void pollAndDraw(const FrameDimensions& frame);
};


#endif // VN_SPRITE_LAYER_H
