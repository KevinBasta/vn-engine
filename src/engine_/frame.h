#ifndef OPENGL_FRAME_H
#define OPENGL_FRAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

class OpenGLFrame {
private:
	GLFWwindow* m_window{};
	FlyCamera m_camera{};
	float m_frameWidth{ 800.0f };
	float m_frameHeight{ 600.0f };

	void initFrame();
	void processInput();

	//void updateResolution();

public:
	OpenGLFrame();
	void gameLoop();


};


#endif // OPENGL_FRAME_H
