#ifndef VN_GAME_CONTEXT_H
#define VN_GAME_CONTEXT_H

#include "model_subject.h"
#include "texture.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GameContext {
public:
	GLFWwindow* m_window{};
	Shader m_defaultShader;

public:
	GameContext(GLFWwindow* window);
	void drawBackground(Texture2D* texture);
	void drawSprite(Texture2D* texture);

	//void reloadShaders();
	/*if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		characterShader.reload();
		m_camera.resetCamera();
	}*/
};

#endif // VN_GAME_CONTEXT_H
