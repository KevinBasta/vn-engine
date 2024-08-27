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

#define TEMP_TEXT_VERTEX_SHADER		VN_BASE_PATH"/src/view/glsl/vertex_text.glsl"
#define TEMP_TEXT_FRAGMENT_SHADER	VN_BASE_PATH"/src/view/glsl/fragment_text.glsl"


class ChoiceLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader m_textShader;

	void drawChoice(const FrameDimensions& frame, std::wstring_view text, glm::vec3 color, float& paddingBottom) {
		if (text == L"") {
			return;
		}

		m_textShader.use();

		glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), color.x, color.y, color.z);

		float scale{ 0.7f };

		// TODO: move ortho to camera object
		glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(frame.width), 0.0f, static_cast<float>(frame.height), 0.0f, 100.0f);
		unsigned int orthoLocation = glGetUniformLocation(m_textShader.ID(), "inOrtho");
		glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));


		float paddingLeft{ 300.0f };
		float paddingRight{ 300.0f };
		int lastConsumedIndex{ 0 };

		std::vector<std::wstring_view> lines{ TextTexture::fittedScreenLines(text, frame.width - ((paddingLeft + paddingRight) * frame.scale), frame.scale * scale) };

		std::vector<std::wstring_view>::iterator line{ lines.begin() };
		for (; line != lines.end(); line++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(paddingLeft * frame.scale, paddingBottom * frame.scale, -1.0f));
			model = glm::scale(model, glm::vec3(frame.scale * scale, frame.scale * scale, 0.0f));
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

	void pollAndDraw(const FrameDimensions& frame) {
		if (m_stateSubject->m_choices.isChoiceActive()) {
			ActionChoice* choices{ m_stateSubject->m_choices.getChoices() };

			if (choices == nullptr) {
				return;
			}

			int index{ 0 };
			int selected{ m_stateSubject->m_choices.getChoiceIndex() };
			float paddingBottom{ 200.0f };
			for (std::wstring_view choice : choices->m_choices) {
				glm::vec3 color{};
				
				if (index == selected) {
					color = { 1.0f, 0.0f, 0.0f };
				}

				drawChoice(frame, choice, color, paddingBottom);
				index++;
			}
		}
	}
};


#endif // VN_LAYER_CHOICE_H
