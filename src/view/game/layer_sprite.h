#ifndef VN_SPRITE_LAYER_H
#define VN_SPRITE_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_delta.h"
#include "state_subject.h"

#define TEMP_VERTEX_PATH	"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\defaultVertex.glsl"
#define TEMP_FRAGMENT_PATH	"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\defaultFragment.glsl"

class SpriteLayer {
private:
	VnWindow* m_window{ nullptr };
	Shader m_defaultShader;
	StateSubject* m_stateSubject{ nullptr };

	void drawSprite(Texture2D* texture) {
		if (texture == nullptr) {
			return;
		}

		m_defaultShader.use();

		float scale{ 0.001f };
		float scaledWidth{ texture->width() * scale };
		float scaledHeight{ texture->height() * scale };

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0 * (scaledWidth / 2.0f), -1.0 * (scaledHeight / 2.0f), -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, 0.0f));

		unsigned int modelLocation = glGetUniformLocation(m_defaultShader.ID(), "inModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		texture->draw();
	}

public:
	SpriteLayer(VnWindow* window, StateSubject* stateSubject):
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH }
	{
	}

	void pollAndDraw() {
		if (m_stateSubject->isInDelta(StateDelta::SPRITE)) {
			StateSubject::spriteRenderMap& data = m_stateSubject->getSpriteRenderData();
			
			drawSprite(data[1].m_texture);
			std::cout << "SPRITE IN DELTA" << std::endl;
		}
	}
};


#endif // VN_SPRITE_LAYER_H
