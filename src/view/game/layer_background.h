#ifndef VN_BACKGROUND_LAYER_H
#define VN_BACKGROUND_LAYER_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_subject.h"

#define TEMP_VERTEX_PATH	"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\defaultVertex.glsl"
#define TEMP_FRAGMENT_PATH	"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\defaultFragment.glsl"

class BackgroundLayer {
private:
	VnWindow* m_window{ nullptr };
	Shader m_defaultShader;
	StateSubject* m_stateSubject{ nullptr };

	// temp
	float m_frameWidth{ 800.0f };
	float m_frameHeight{ 600.0f };

	void drawBackground(Texture2D* texture) {
		if (texture == nullptr) {
			return;
		}

		m_defaultShader.use();

		float imageWidth{ static_cast<float>(texture->width()) };
		float imageHeight{ static_cast<float>(texture->height()) };

		// can move to Texture2D
		float scaleFactor{ 0 };
		float scaleDown{};

		if (imageWidth > imageHeight) {
			scaleDown = 1 / (std::max(imageWidth, m_frameWidth) - std::min(imageWidth, m_frameWidth));
		}
		else {
			scaleDown = 1 / (std::max(imageHeight, m_frameHeight) - std::min(imageHeight, m_frameHeight));
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(scaleDown, scaleDown, 0.0f));
		model = glm::translate(model, glm::vec3(-1 * (imageWidth / 2), -1 * (imageHeight / 2), -1.0f));

		unsigned int modelLocation = glGetUniformLocation(m_defaultShader.ID(), "inModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		texture->draw();
	}

public:
	BackgroundLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH }
	{
	}

	void pollAndDraw() {
		if (m_stateSubject->isInDelta(StateDelta::BACKGROUND)) {
			drawBackground(m_stateSubject->m_currentBackground);
			std::cout << "BACKGROUND IN DELTA" << std::endl;
		}
	}
};



#endif // VN_BACKGROUND_LAYER_H
