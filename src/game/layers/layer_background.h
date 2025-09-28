#ifndef VN_BACKGROUND_LAYER_H
#define VN_BACKGROUND_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_subject.h"
#include "node_types.h"

class BackgroundLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader* m_textureShader{ nullptr };

	void drawBackground(const FrameDimensions& frame, TextureIdentifier& texture, BackgroundOffsets& offsets);

public:
	BackgroundLayer(VnWindow* window, StateSubject* stateSubject, Shader* textureShader) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textureShader{ textureShader }
	{
	}

	void pollAndDraw(const FrameDimensions& frame);
	void draw(const FrameDimensions& frame, TextureIdentifier texture, BackgroundOffsets offsets = {});
};



#endif // VN_BACKGROUND_LAYER_H
