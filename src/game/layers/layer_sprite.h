#ifndef VN_SPRITE_LAYER_H
#define VN_SPRITE_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"

class SpriteLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader* m_textureShader{ nullptr };

	void drawSprite(const FrameDimensions& frame, const TextureIdentifier& textureIdentifier, const SpriteState& spriteState);

public:
	SpriteLayer(VnWindow* window, StateSubject* stateSubject, Shader* textureShader):
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textureShader{ textureShader }
	{
	}

	void pollAndDraw(const FrameDimensions& frame);
};


#endif // VN_SPRITE_LAYER_H
