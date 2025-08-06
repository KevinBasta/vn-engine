
#include "layer_background.h"

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_subject.h"
#include "state_background.h"
#include "node_types.h"

void BackgroundLayer::drawBackground(const FrameDimensions& frame, TextureIdentifier& textureIdentifier) {
	m_defaultShader.use();

	Texture2D* texture{ TextureManager::getTexture(textureIdentifier) };

	if (texture == nullptr) {
		return;
	}

	// can move to Texture2D
	float scale{ texture->getScaleToFrame(frame.width, frame.height) };

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -99.0f));
	model = glm::scale(model, glm::vec3(scale, scale, 0.0f));

	unsigned int modelLocation = glGetUniformLocation(m_defaultShader.ID(), "inModel");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	unsigned int viewLocation = glGetUniformLocation(m_defaultShader.ID(), "inView");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


	glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(frame.width), 0.0f, static_cast<float>(frame.height), 0.0f, 100.0f);

	unsigned int orthoLocation = glGetUniformLocation(m_defaultShader.ID(), "inOrtho");
	glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));

	unsigned int opacityLocation = glGetUniformLocation(m_defaultShader.ID(), "inOpacity");
	glUniform1f(opacityLocation, 1.0f);

	texture->draw();
}

void BackgroundLayer::pollAndDraw(const FrameDimensions& frame) {
	drawBackground(frame, m_stateSubject->m_background.m_currentBackground);

	if (m_stateSubject->isInDelta(StateDelta::BACKGROUND)) {
		std::cout << "BACKGROUND IN DELTA" << std::endl;
	}
}