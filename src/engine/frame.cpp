
#include <iostream>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "runtime_characters.h"
#include "shader.h"
#include "frame.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

FlyCamera* g_camera{};

static void frameSizeUpdateCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	(*g_camera).processMouseMovement(xpos, ypos);
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	(*g_camera).processMouseScroll(yoffset);
}

void OpenGLFrame::initFrame() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_frameWidth, m_frameHeight, "opengl :)", NULL, NULL);
	if (m_window == NULL) {
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		// raise exception or return err?
	}
	
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, frameSizeUpdateCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		// raise exception or return err?
	}

	// Uncomment to enable transparent backgrounds
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, m_frameWidth, m_frameHeight);

	g_camera = &m_camera;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, mouseCallback);
	glfwSetScrollCallback(m_window, scrollCallback);
}


void OpenGLFrame::processInput() {
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

OpenGLFrame::OpenGLFrame() {
	initFrame();
}

void OpenGLFrame::gameLoop() {

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	Shader characterShader(
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Glsl\\characterVertex.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Glsl\\characterFragment.glsl");
	
	Shader brackgroundShader(
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Glsl\\backgroundVertex.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Glsl\\backgroundFragment.glsl");

	Texture2D& characterTexture{ g_characters[0].get()->getTextures()[0] };

	Texture2D backgroundTexture{ "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\test.jpg" };

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

		glm::mat4 model = glm::mat4(1.0f);

		float widthRatio{ m_frameWidth / backgroundTexture.width() };
		float heightRatio{ m_frameHeight / backgroundTexture.height() };

		float bestRatio{ std::min(widthRatio, heightRatio) };

		float screenAspect{ m_frameWidth / m_frameHeight };
		float imageAspect{ static_cast<float>(backgroundTexture.width()) / backgroundTexture.height() };
		float scaleFactor{ 0 };

		float imageWidth	{ static_cast<float>(backgroundTexture.width()) };
		float imageHeight	{ static_cast<float>(backgroundTexture.height()) };

		float widthScaled	{ 0 };
		float heightScaled	{ 0 };

		if (screenAspect > imageAspect) {
			widthScaled		= imageWidth * (m_frameHeight / imageHeight);
			heightScaled	= m_frameHeight;
		} 
		else {
			widthScaled		= m_frameWidth;
			heightScaled	= imageHeight * (m_frameWidth / imageWidth);
		}
		
		float scaleDown{ 1 / (imageWidth - m_frameWidth)};
		model = glm::scale(model, glm::vec3(scaleDown, scaleDown, 0.0f));
		model = glm::translate(model, glm::vec3(-1 * (imageWidth / 2), -1 * (imageHeight / 2), -1.0f));


		//model = glm::translate(model, glm::vec3(-1.0 * (scaledWidth1 / 2.0f), -1.0 * (scaledHeight1 / 2.0f), -1.0f));

		glUniformMatrix4fv(bgModelLocation, 1, GL_FALSE, glm::value_ptr(model));


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