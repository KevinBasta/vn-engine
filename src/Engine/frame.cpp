
#include <iostream>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "runtime_characters.h"
#include "shader.h"
#include "frame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static void frameSizeUpdateCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

void OpenGLFrame::initFrame() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(800, 600, "opengl :)", NULL, NULL);
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, 800, 600);
}


void OpenGLFrame::processInput() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}

	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		// reload all shaders?
	}
}


void OpenGLFrame::setupRectangle() {
	
}

void OpenGLFrame::drawRectangle() {

}

OpenGLFrame::OpenGLFrame() {
	initFrame();
}

void OpenGLFrame::gameLoop() {
	OpenGLShader squareShader(
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Engine\\scene_vertex_shader.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Engine\\scene_fragment_shader.glsl");

	int width	{ g_characters[0].get()->getTextures()[0].width() };
	int height	{ g_characters[0].get()->getTextures()[0].height() };

	std::pair<float, float> topLeft		{ 0.0								, 0.0 };
	std::pair<float, float> topRight	{ static_cast<float>(width) / 800.0f	, 0.0 };
	std::pair<float, float> bottomLeft	{ 0.0								, static_cast<float>(height) / 600.0f };
	std::pair<float, float> buttomRight	{ static_cast<float>(width) / 800.0f	, static_cast<float>(height) / 600.0f };

	float vertices[] = {
		// positions				// texture coords
		  topRight.first,  topRight.second, 0.0f,		0.0f, 1.0f,			// top right
		  buttomRight.first, -1 * buttomRight.second, 0.0f,		0.0f, 0.0f,			// bottom right
		 -1 * bottomLeft.first, -1 * bottomLeft.second, 0.0f,		1.0f, 0.0f,			// bottom left
		 -1 * topLeft.first,  topLeft.second, 0.0f,		1.0f, 1.0f,			// top left
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 3, 2
	};
	
	// vertex array object, vertex buffer object, Element Buffer Object
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Connect everything to the VAO
	glBindVertexArray(VAO);

	// vertex data buffer, put data in a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// define what points are used to make the shapes, reusing vertecies from VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// define how the different attributes data are stored

	// vertex coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture mapping
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	


	squareShader.use();
	glUniform1i(glGetUniformLocation(squareShader.m_programID, "inTexture"), 0);
	

	while (!glfwWindowShouldClose(m_window)) {
		processInput();

		if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
			squareShader.reload();
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		squareShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	glfwTerminate();
	// return?
}