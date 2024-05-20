#ifndef VN_OPENGL_WINDOW_H
#define VN_OPENGL_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

static void frameSizeUpdateCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

class VnWindow {
private:
	GLFWwindow* m_window{};
	float m_frameWidth{ 800.0f };
	float m_frameHeight{ 600.0f };

public:
	VnWindow() {}

	// load glfw, create window, and load glad
	void load() {
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

		glViewport(0, 0, m_frameWidth, m_frameHeight);

		std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
	}

	GLFWwindow* getWindow() {
		return m_window;
	}
};


#endif // VN_OPENGL_WINDOW_H