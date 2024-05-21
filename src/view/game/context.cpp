
#include "shader.h"
#include "context.h"
#include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <utility>

#include "model_subject.h"



FlyCamera* g_camera{};


GameContext::GameContext(GLFWwindow* window) :
	m_window{ window },
	m_characterShader{ 
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\backgroundVertex.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\backgroundFragment.glsl" },
	m_brackgroundShader{
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\backgroundVertex.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\backgroundFragment.glsl"}
{
	// Uncomment to enable transparent backgrounds
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

void GameContext::drawBackground(Texture2D* texture) {
	if (texture == nullptr) {
		return;
	}

	m_brackgroundShader.use();


	float imageWidth{ static_cast<float>(texture->width()) };
	float imageHeight{ static_cast<float>(texture->height()) };

	// can move to Texture2D
	float scaleFactor{ 0 };
	float scaleDown{};

	if (imageWidth > imageHeight) {
		scaleDown = 1 / (std::max(imageWidth, m_frameWidth) - std::min(imageWidth, m_frameWidth));
	}
	else {
		scaleDown = 1 / (std::max(imageHeight, m_frameHeight) - std::min(imageHeight, m_frameHeight));
	}


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(scaleDown, scaleDown, 0.0f));
	model = glm::translate(model, glm::vec3(-1 * (imageWidth / 2), -1 * (imageHeight / 2), -1.0f));


	unsigned int modelLocation = glGetUniformLocation(m_brackgroundShader.ID(), "inModel");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));


	texture->draw();
}

void GameContext::drawSprite(Texture2D* texture) {
	if (texture == nullptr) {
		return;
	}

	m_characterShader.use();

	float scale{ 0.001f };
	float scaledWidth{ texture->width() * scale };
	float scaledHeight{ texture->height() * scale };
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0 * (scaledWidth / 2.0f), -1.0 * (scaledHeight / 2.0f), -1.0f));
	model = glm::scale(model, glm::vec3(scale, scale, 0.0f));

	unsigned int modelLocation = glGetUniformLocation(m_characterShader.ID(), "inModel");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	texture->draw();
}
