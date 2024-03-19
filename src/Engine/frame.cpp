
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "frame.h"

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
}


void OpenGLFrame::gameLoop() {
	initFrame();


	while (!glfwWindowShouldClose(m_window)) {
		processInput();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	glfwTerminate();
	// return?
}