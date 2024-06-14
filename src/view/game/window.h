#ifndef VN_OPENGL_WINDOW_H
#define VN_OPENGL_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define widthRatio 16
#define heightRatio 9

// TODO: Must turn into singleton so that sg_ variables apply only to that one
// instance of VnWindow


// TODO: Need to decide if standard window size should be 1600x900 or 1920x1080

#define standardWindowWidth 1920.0f
#define standardWindowHeight 1080.0f

static int sg_frameWidth{ 1920 };
static int sg_frameHeight{ 1080 };


// The nodes will have sprite positions that are relative 
// to the standard window width and height
// scale is used to convert those positions to the new
// width and height resized to

static float sg_scale{ 1.0f };
static bool sg_updated{ true };

static void frameSizeUpdateCallback(GLFWwindow* window, int newWidth, int newHeight) {
	/*
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
	*/


	// TODO: Need to handle window resize to draw during resize?
	// Also need to handle aspect ratio being forced out of 
	// what it's set to (i.e. in the case that windows snap is used
	// by perhaps truncating to nearest aspect ratio compliant width/height 

	// calculate how much in the x axis or y axis we are off by
	// then make glViewport use the first two value to offset half
	// of that so that the viewport is centered

	glfwSetWindowAspectRatio(window, widthRatio, heightRatio);
	glViewport(0, 0, newWidth, newHeight);

	sg_scale = newWidth / standardWindowWidth;

	sg_frameWidth = newWidth;
	sg_frameHeight = newHeight;

	sg_updated = true;
}

static void windowMaximizeCallback(GLFWwindow* window, int maximized) {
	// https://www.glfw.org/docs/3.3/window_guide.html
	// TODO: disable the maximize button as it messes up the ratio
	// TODO: make it so only can set window to preselected percentages or sizes
	// or just full screen
	// TODO: center the viewport and make equal bars on left and right if
	// a device has difference aspect ratio
	
	/*if (maximized) {
		glfwSetWindowSize(window, sg_frameWidth, sg_frameHeight);
	}*/
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
		glfwSetWindowMaximizeCallback(m_window, windowMaximizeCallback);



		// fullscreen 
		// glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, sg_frameWidth, sg_frameHeight, GLFW_DONT_CARE);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "failed to initialize GLAD" << std::endl;
			// raise exception or return err?
		}

		glViewport(0, 0, sg_frameWidth, sg_frameHeight);
		//glfwMaximizeWindow(m_window);

		std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
	}

	int width() {
		return sg_frameWidth;
	}

	int height() {
		return sg_frameHeight;
	}

	float scale() {
		return sg_scale;
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