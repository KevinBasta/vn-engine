﻿#ifndef VN_LAYER_TEXT_H
#define VN_LAYER_TEXT_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_subject.h"
#include "text.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <string_view>
#include <iostream>
#include <unordered_map>

#define TEMP_TEXT_VERTEX_SHADER		VN_BASE_PATH"/src/view/glsl/vertex_text.glsl"
#define TEMP_TEXT_FRAGMENT_SHADER	VN_BASE_PATH"/src/view/glsl/fragment_text.glsl"

class TextLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader m_textShader;

	// auto const joined = std::views::join(std::array{s1, s2});
	// can join string if want to display text and name on one line
	void drawText(const FrameDimentions frameDimentions, std::wstring_view text, glm::vec3 color, float paddingBottom) {

		// TODO: gracefully handle empty text variable. A crash occurs in that case at the moment.
		if (text == L"") {
			return;
		}

		m_textShader.use();
		
		glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), color.x, color.y, color.z);

		float scale{ 0.7f };

		// TODO: move ortho to camera object
		glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(frameDimentions.frameWidth), 0.0f, static_cast<float>(frameDimentions.frameHeight), 0.0f, 100.0f);
		unsigned int orthoLocation = glGetUniformLocation(m_textShader.ID(), "inOrtho");
		glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));


		float paddingLeft{ 300.0f };
		float paddingRight{ 300.0f };
		int lastConsumedIndex{ 0 };
		
		//std::wstring testText = L"Hello, this is Garu. I've come from a far land. To meet brazazazaza. brazazaza Test Test Test how should line breaking work?";
		
		std::vector<std::wstring_view> lines{ TextTexture::fittedScreenLines(text, frameDimentions.frameWidth - ((paddingLeft + paddingRight) * frameDimentions.scale), frameDimentions.scale * scale) };

		std::vector<std::wstring_view>::iterator line{ lines.begin() };
		for (; line != lines.end(); line++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(paddingLeft * frameDimentions.scale, paddingBottom * frameDimentions.scale, -1.0f));
			model = glm::scale(model, glm::vec3(frameDimentions.scale * scale, frameDimentions.scale * scale, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(m_textShader.ID(), "inModel"), 1, GL_FALSE, glm::value_ptr(model));

			TextTexture::draw(*line);

			paddingBottom -= 50.0f;
		}
	}

public:
	TextLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textShader{ TEMP_TEXT_VERTEX_SHADER, TEMP_TEXT_FRAGMENT_SHADER }
	{
	}

	void pollAndDraw(const FrameDimentions frameDimentions) {
		TextState& state{ m_stateSubject->m_dialogue.get() };

		if (state.m_render != false) {
			drawText(frameDimentions,
				state.m_speakerName,
				state.m_color,
				200.0f);
			drawText(frameDimentions,
				state.m_line,
				state.m_color,
				150.0f);
		}
	}
};

#endif // VN_LAYER_TEXT_H
