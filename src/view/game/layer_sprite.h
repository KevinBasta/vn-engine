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

		float scale{ texture->getScaleToViewport(m_window) };

		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3((static_cast<float>(m_window->width()) / 2) - (static_cast<float>(texture->width()) / 2), 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1500.0f * m_window->scale(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, 0.0f));

		unsigned int modelLocation = glGetUniformLocation(m_defaultShader.ID(), "inModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

		unsigned int viewLocation = glGetUniformLocation(m_defaultShader.ID(), "inView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


		glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(m_window->width()), 0.0f, static_cast<float>(m_window->height()), 0.0f, 100.0f);

		unsigned int orthoLocation = glGetUniformLocation(m_defaultShader.ID(), "inOrtho");
		glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));


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
		StateSubject::spriteRenderMap& data = m_stateSubject->getSpriteRenderData();
		
		StateSubject::spriteRenderMap::iterator iter;
		for (iter = data.begin(); iter != data.end(); iter++) {
			if (iter->second.m_texture != nullptr && iter->second.m_opacity > 0.0f) {
				drawSprite(iter->second.m_texture);
			}
		}

		if (m_stateSubject->isInDelta(StateDelta::SPRITE)) {
			
			std::cout << "SPRITE IN DELTA" << std::endl;
		}
	}
};


#endif // VN_SPRITE_LAYER_H
