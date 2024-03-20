
#include <iostream>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "frame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void OpenGLFrame::gameLoop() {
	initFrame();

	OpenGLShader squareShader(
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Engine\\scene_vertex_shader.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Engine\\scene_fragment_shader.glsl");


	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and geneate the texture
	int width, height, nrChannels;
	char filepath[] = "C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\assets\\garu_outline.png";
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture" << std::endl;
	}

	stbi_image_free(data);


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