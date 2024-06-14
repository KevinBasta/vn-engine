#ifndef VN_OPENGL_WINDOW_H
#define VN_OPENGL_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define widthRatio 16
#define heightRatio 9

// TODO: Must turn into singleton so that sg_ variables apply only to that one
// instance of VnWindow
static int sg_frameWidth{ 1600 };
static int sg_frameHeight{ 900 };
static bool sg_updated{ true };

static void frameSizeUpdateCallback(GLFWwindow* window, int newWidth, int newHeight) {
	int ratioWidth{ 0 };
	int ratioHeight{ 0 };
	
	float ratioMultiplier{ 0.0f };

	if (newWidth > newHeight) {
		ratioMultiplier = newHeight / heightRatio;
		
		ratioWidth = ratioMultiplier * widthRatio;
		ratioHeight = newHeight;
	}
	else {
		ratioMultiplier = newWidth / widthRatio;

		ratioWidth = newWidth;
		ratioHeight = ratioMultiplier * heightRatio;
	}
	
	glViewport(0, 0, ratioWidth, ratioHeight);

	sg_frameWidth = ratioWidth;
	sg_frameHeight = ratioHeight;
	sg_updated = true;
}

class VnWindow {
private:
	GLFWwindow* m_window{};

public:
	VnWindow() {}
	~VnWindow() { 
		std::cout << "VN WINDOW DYSTROYED" << std::endl;
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	// load glfw, create window, and load glad
	void load() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(sg_frameWidth, sg_frameHeight, "opengl :)", NULL, NULL);
		if (m_window == NULL) {
			std::cout << "failed to create GLFW window" << std::endl;
			glfwTerminate();
			// raise exception or return err?
		}

		glfwMakeContextCurrent(m_window);
		glfwSetWindowAspectRatio(m_window, widthRatio, heightRatio);
		glfwSetFramebufferSizeCallback(m_window, frameSizeUpdateCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "failed to initialize GLAD" << std::endl;
			// raise exception or return err?
		}

		glViewport(0, 0, sg_frameWidth, sg_frameHeight);

		std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
	}

	int width() {
		return sg_frameWidth;
	}

	int height() {
		return sg_frameHeight;
	}

	bool updated() {
		bool updated{ sg_updated };

		sg_updated = false;
		
		return updated;
	}

	GLFWwindow* get() {
		return m_window;
	}

	GLFWwindow* operator()() {
		return m_window;
	}
};


#endif // VN_OPENGL_WINDOW_H