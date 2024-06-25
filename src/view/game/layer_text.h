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

	}

	void drawText() {
		m_textShader.use();
		
		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), color.x, color.y, color.z);

		float scalee{ 0.001f };
		float scaledWidth{ 100 * scalee };
		float scaledHeight{ 10 * scalee };

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(300.0f * m_window->scale(), 100.0f * m_window->scale(), -1.0f));
		model = glm::scale(model, glm::vec3(m_window->scale(), m_window->scale(), 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(m_textShader.ID(), "inModel"), 1, GL_FALSE, glm::value_ptr(model));


		// TODO: move ortho to vn window
		glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(m_window->width()), 0.0f, static_cast<float>(m_window->height()), 0.0f, 100.0f);

		unsigned int orthoLocation = glGetUniformLocation(m_textShader.ID(), "inOrtho");
		glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));

		TextTexture::draw(L"test test test lol 123 あ私 test hello this is the text area does this wrap around????? idk but needs to");
	}

public:
	TextLayer(VnWindow* window, StateSubject* stateSubject) :
		m_window{ window },
		m_stateSubject{ stateSubject },
		m_textShader{ TEMP_TEXT_VERTEX_SHADER, TEMP_TEXT_FRAGMENT_SHADER }
	{
	}

	void pollAndDraw() {
		drawText();
	}
};

#endif // VN_LAYER_TEXT_H
