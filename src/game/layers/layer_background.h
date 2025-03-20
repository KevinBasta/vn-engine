#ifndef VN_BACKGROUND_LAYER_H
#define VN_BACKGROUND_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_subject.h"
#include "chapter_node_types.h"

#define TEMP_VERTEX_PATH	VN_BASE_PATH"/src/game_internals/glsl/vertex_texture.glsl"
#define TEMP_FRAGMENT_PATH	VN_BASE_PATH"/src/game_internals/glsl/fragment_texture.glsl"

class BackgroundLayer {
private:
	VnWindow* m_window{ nullptr };
	Shader m_defaultShader;
	StateSubject* m_stateSubject{ nullptr };

	void drawBackground(const FrameDimensions& frame, TextureIdentifier& texture);

public:
	BackgroundLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH }
	{
	}

	void pollAndDraw(const FrameDimensions& frame);
};



#endif // VN_BACKGROUND_LAYER_H
