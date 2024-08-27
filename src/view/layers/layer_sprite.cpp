
#include "layer_sprite.h"

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "state_types.h"
#include "state_sprites.h"
#include "state_subject.h"

void SpriteLayer::drawSprite(const FrameDimensions& frame, const TextureIdentifier& textureIdentifier, const SpriteState& spriteState) {
	m_defaultShader.use();

	Texture2D* texture{ TextureManager::getTexture(textureIdentifier) };

	if (texture == nullptr) {
		return;
	}

	float scale{ texture->getScaleToFrame(frame.width, frame.height) * spriteState.m_scale };

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3((static_cast<float>(m_window->width()) / 2) - (static_cast<float>(texture->width()) / 2), 0.0f, 0.0f));
	model = glm::translate(model,
		glm::vec3(spriteState.m_xPos * frame.scale,
				  spriteState.m_yPos * frame.scale,
				  spriteState.m_zPos - 10.0f));

	//std::cout << "scale to view port" << scale << std::endl;
	//std::cout << "scale to view port" << spriteState.m_position.m_xCoord << std::endl;
	//std::cout << "scale to view port" << spriteState.m_position.m_yCoord << std::endl;
	//std::cout << "scale to view port" << spriteState.m_position.m_zCoord << std::endl;
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
	glUniform1f(opacityLocation, spriteState.m_opacity);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	texture->draw();
}


void SpriteLayer::pollAndDraw(const FrameDimensions& frame) {
	auto& data{ m_stateSubject->m_sprites.getSpriteRenderData() };

	auto iter{ data.begin() };
	for (; iter != data.end(); iter++) {
		if (iter->second.m_opacity > 0.0f) {
			drawSprite(frame, iter->first, iter->second);
		}
	}

	if (m_stateSubject->isInDelta(StateDelta::SPRITE)) {
		std::cout << "SPRITE IN DELTA" << std::endl;
	}
}
