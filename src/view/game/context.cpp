
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

float deltaTime = 0.0f;
float lastFrame = 0.0f;

FlyCamera* g_camera{};


static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	(*g_camera).processMouseMovement(xpos, ypos);
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	(*g_camera).processMouseScroll(yoffset);
}

void GameContext::initFrame() {
	// Uncomment to enable transparent backgrounds
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	g_camera = &m_camera;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, mouseCallback);
	glfwSetScrollCallback(m_window, scrollCallback);
}


void GameContext::processInput() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}

	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		// reload all shaders?
	}

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_W, deltaTime);
	}

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_S, deltaTime);
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_D, deltaTime);
	}

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_A, deltaTime);
	}
}



void GameContext::gameLoop() {

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	Shader characterShader(
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\characterVertex.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\characterFragment.glsl");
	
	Shader brackgroundShader(
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\backgroundVertex.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\backgroundFragment.glsl");

	Texture2D& characterTexture{ m_subject->m_characters[0].get()->getTextures()[0] };

	Texture2D backgroundTexture{ "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\test.jpg", TextureType::BACKGROUND };

	characterShader.use();
	glUniform1i(glGetUniformLocation(characterShader.ID(), "inTexture"), 0);
	
	unsigned int viewLocation = glGetUniformLocation(characterShader.ID(), "inView");
	unsigned int projectionLocation = glGetUniformLocation(characterShader.ID(), "inProjection");

	glm::mat4 projection = glm::perspective(90.0f, m_frameWidth / m_frameHeight, 0.0f, 1000.0f);


	while (!glfwWindowShouldClose(m_window)) {
		processInput();

		if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
			characterShader.reload();
			m_camera.resetCamera();
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// background
		brackgroundShader.use();

		unsigned int bgModelLocation = glGetUniformLocation(brackgroundShader.ID(), "inModel");
		unsigned int bgViewLocation = glGetUniformLocation(brackgroundShader.ID(), "inView");

		glm::mat4 model = glm::mat4(1.0f);

		float screenAspect{ m_frameWidth / m_frameHeight };
		float imageAspect{ static_cast<float>(backgroundTexture.width()) / backgroundTexture.height() };

		float imageWidth	{ static_cast<float>(backgroundTexture.width()) };
		float imageHeight	{ static_cast<float>(backgroundTexture.height()) };

		float scaleFactor{ 0 };
		float scaleDown{};

		if (imageWidth > imageHeight) {
			scaleDown = 1 / (std::max(imageWidth, m_frameWidth) - std::min(imageWidth, m_frameWidth));
		} 
		else {
			scaleDown = 1 / (std::max(imageHeight, m_frameHeight) - std::min(imageHeight, m_frameHeight));
		}
		
		model = glm::scale(model, glm::vec3(scaleDown, scaleDown, 0.0f));
		model = glm::translate(model, glm::vec3(-1 * (imageWidth / 2), -1 * (imageHeight / 2), -1.0f));


		//model = glm::translate(model, glm::vec3(-1.0 * (scaledWidth1 / 2.0f), -1.0 * (scaledHeight1 / 2.0f), -1.0f));

		glUniformMatrix4fv(bgModelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(bgViewLocation, 1, GL_FALSE, glm::value_ptr(m_camera.getViewMatrix()));


		backgroundTexture.draw();


		// character
		characterShader.use();

		unsigned int chModelLocation = glGetUniformLocation(characterShader.ID(), "inModel");

		float scale{ 0.004f };
		float scaledWidth{ characterTexture.width() * scale };
		float scaledHeight{ characterTexture.height() * scale };
		// set coordinate system matrix uniforms
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0 * (scaledWidth / 2.0f), -1.0 * (scaledHeight / 2.0f), -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, 0.0f));
		
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_camera.getViewMatrix()));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(chModelLocation, 1, GL_FALSE, glm::value_ptr(model));

		characterTexture.draw();


		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	glfwTerminate();
	// return?
}