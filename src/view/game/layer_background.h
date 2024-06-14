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

	void drawBackground(Texture2D* texture) {
		if (texture == nullptr) {
			return;
		}

		m_defaultShader.use();

		// can move to Texture2D
		float scale{ texture->getScaleToViewport(m_window) };

		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f));
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
		drawBackground(m_stateSubject->m_currentBackground);
		
		if (m_stateSubject->isInDelta(StateDelta::BACKGROUND)) {
			std::cout << "BACKGROUND IN DELTA" << std::endl;
		}
	}
};



#endif // VN_BACKGROUND_LAYER_H
