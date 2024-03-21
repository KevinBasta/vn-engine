#ifndef OPENGL_FRAME_H
#define OPENGL_FRAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLFrame {
private:
	GLFWwindow* m_window{};
	int m_frameWidth{};
	int m_frameHeight{};

	void initFrame();
	void processInput();

	void setupRectangle();
	void drawRectangle();
	//void updateResolution();

public:
	OpenGLFrame();
	void gameLoop();


};



#endif // OPENGL_FRAME_H
