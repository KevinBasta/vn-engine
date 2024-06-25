
#include "layer_background.h"

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_subject.h"

void BackgroundLayer::drawBackground(Texture2D* texture) {
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

void BackgroundLayer::pollAndDraw() {
	drawBackground(m_stateSubject->m_currentBackground);

	if (m_stateSubject->isInDelta(StateDelta::BACKGROUND)) {
		std::cout << "BACKGROUND IN DELTA" << std::endl;
	}
}