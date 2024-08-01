#ifndef VN_LAYER_CHOICE_H
#define VN_LAYER_CHOICE_H

#include "window.h"
#include "chapter_node_types.h"
#include "state_subject.h"
#include "shader.h"
#include "text.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <string_view>
#include <iostream>

#define TEMP_TEXT_VERTEX_SHADER		VN_BASE_PATH"/src/view/glsl/textVertex.glsl"
#define TEMP_TEXT_FRAGMENT_SHADER	VN_BASE_PATH"/src/view/glsl/textFragment.glsl"


class ChoiceLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader m_textShader;

	void drawChoice(std::wstring_view text, glm::vec3 color, float& paddingBottom) {
		if (text == L"") {
			return;
		}

		m_textShader.use();

		glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), color.x, color.y, color.z);

		float scale{ 0.7f };

		// TODO: move ortho to camera object
		glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(m_window->width()), 0.0f, static_cast<float>(m_window->height()), 0.0f, 100.0f);
		unsigned int orthoLocation = glGetUniformLocation(m_textShader.ID(), "inOrtho");
		glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));


		float paddingLeft{ 300.0f };
		float paddingRight{ 300.0f };
		int lastConsumedIndex{ 0 };

		std::vector<std::wstring_view> lines{ TextTexture::fittedScreenLines(text, m_window->width() - ((paddingLeft + paddingRight) * m_window->scale()), m_window->scale() * scale) };

		std::vector<std::wstring_view>::iterator line{ lines.begin() };
		for (; line != lines.end(); line++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(paddingLeft * m_window->scale(), paddingBottom * m_window->scale(), -1.0f));
			model = glm::scale(model, glm::vec3(m_window->scale() * scale, m_window->scale() * scale, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(m_textShader.ID(), "inModel"), 1, GL_FALSE, glm::value_ptr(model));

			TextTexture::draw(*line);

			paddingBottom -= 50.0f;
		}
	}

public:
	ChoiceLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textShader{ TEMP_TEXT_VERTEX_SHADER, TEMP_TEXT_FRAGMENT_SHADER }
	{
	}

	void pollAndDraw() {
		if (m_stateSubject->m_choices.isChoiceActive()) {
			ActionChooseNode* choices{ m_stateSubject->m_choices.getNodeChoices() };

			if (choices == nullptr) {
				return;
			}

			int index{ 0 };
			int selected{ m_stateSubject->m_choices.getChoiceIndex() };
			float paddingBottom{ 200.0f };
			for (ChoiceTextProperties& choice : choices->m_choices) {
				glm::vec3 color{};
				
				if (index == selected) {
					color = { 1.0f, 0.0f, 0.0f };
				}

				drawChoice(choice.m_displayText, color, paddingBottom);
				index++;
			}
		}
	}
};


#endif // VN_LAYER_CHOICE_H
