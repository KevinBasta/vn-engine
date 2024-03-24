#ifndef OPENGL_FRAME_H
#define OPENGL_FRAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

class OpenGLFrame {
private:
	GLFWwindow* m_window{};
	OpenGLCamera m_camera{};
	int m_frameWidth{};
	int m_frameHeight{};

	void initFrame();
	void processInput();

	//void updateResolution();

public:
	OpenGLFrame();
	void gameLoop();


};


#endif // OPENGL_FRAME_H
