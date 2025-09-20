
#include "layer_saves.h"

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "text.h"
#include "state_subject.h"
#include "node_types.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void SavesMenuLayer::drawOptions(const FrameDimensions& frame) {
	m_textShader.use();

	float scale{ 1.5f };

	// TODO: move ortho to camera object
	glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(frame.width), 0.0f, static_cast<float>(frame.height), 0.0f, 100.0f);
	unsigned int orthoLocation = glGetUniformLocation(m_textShader.ID(), "inOrtho");
	glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));

	float paddingLeft{ 200.0f };
	float paddingRight{ 0.0f };

	float paddingBottom{ 700.0f };

	for (int i{ 0 }; i <= m_stateSubject->m_savesMenu.maxChoice(); i++) {
		if (i == m_stateSubject->m_savesMenu.currentChoice()) {
			glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), 255, 0, 0);
		}
		else {
			glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), 100, 100, 100);
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(paddingLeft * frame.scale, paddingBottom * frame.scale, -1.0f));
		model = glm::scale(model, glm::vec3(frame.scale * scale, frame.scale * scale, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(m_textShader.ID(), "inModel"), 1, GL_FALSE, glm::value_ptr(model));

		TextTexture::draw(m_stateSubject->m_savesMenu.choiceTitleAt(i));

		paddingBottom -= 100.0f;
	}
}
