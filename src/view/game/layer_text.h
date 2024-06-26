#ifndef VN_LAYER_TEXT_H
#define VN_LAYER_TEXT_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_subject.h"
#include "text.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <string_view>
#include <iostream>
#include <unordered_map>

#define TEMP_TEXT_VERTEX_SHADER		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\textVertex.glsl"
#define TEMP_TEXT_FRAGMENT_SHADER	"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\textFragment.glsl"

class TextLayer {
private:
	VnWindow* m_window{ nullptr };
	StateSubject* m_stateSubject{ nullptr };
	Shader m_textShader;

	void drawBackground(Texture2D* texture) {
		if (texture == nullptr) {
			return;
		}


	}

	void drawText(std::wstring_view speaker, std::wstring_view text, glm::vec3 color) {
		//text = L"Hello, this is Garu. I've come from a far land. To meet brazazazaza.\n brazazaza Test Test Test how should line breaking work?";

		m_textShader.use();
		
		glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), color.x, color.y, color.z);

		float scale{ 0.7f };

		// TODO: move ortho to camera object
		glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(m_window->width()), 0.0f, static_cast<float>(m_window->height()), 0.0f, 100.0f);
		unsigned int orthoLocation = glGetUniformLocation(m_textShader.ID(), "inOrtho");
		glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));


		float paddingLeft{ 300.0f };
		float paddingRight{ 300.0f };
		float paddingBottom{ 150.0f };
		int lastConsumedIndex{ 0 };

		while (lastConsumedIndex < text.length() - 1) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(paddingLeft * m_window->scale(), paddingBottom * m_window->scale(), -1.0f));
			model = glm::scale(model, glm::vec3(m_window->scale() * scale, m_window->scale() * scale, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(m_textShader.ID(), "inModel"), 1, GL_FALSE, glm::value_ptr(model));

			int endIndex = TextTexture::computeBreakIndex(text, lastConsumedIndex, m_window->width() - ((paddingLeft + paddingRight) * m_window->scale()), m_window->scale() * scale);
			TextTexture::draw(text.substr(lastConsumedIndex, endIndex - lastConsumedIndex));
			
			// Skip any non-display characters
			while (endIndex < text.length() - 1) {
				wchar_t c{ text[endIndex] };

				//std::cout << int(c) << std::endl;

				if (c == ' ' || c == '\n') {
					endIndex++;
				}
				else {
					break;
				}
			}

			lastConsumedIndex = endIndex;

			paddingBottom -= 50.0f;
			std::cout << "Next start index: " << lastConsumedIndex << std::endl;
		}



	}

public:
	TextLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textShader{ TEMP_TEXT_VERTEX_SHADER, TEMP_TEXT_FRAGMENT_SHADER }
	{
	}

	void pollAndDraw() {
		drawText(m_stateSubject->m_textState.m_speakerName, 
				 m_stateSubject->m_textState.m_line,
				 m_stateSubject->m_textState.m_color);
	}
};

#endif // VN_LAYER_TEXT_H
