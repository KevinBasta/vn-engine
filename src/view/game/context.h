#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H


#include "camera.h"
#include "model_subject.h"
#include "texture.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GameContext {
public:
	GLFWwindow* m_window{};
	FlyCamera m_camera{};
	Shader m_characterShader;
	Shader m_brackgroundShader;

	// temp
	float m_frameWidth{ 800.0f };
	float m_frameHeight{ 600.0f };

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

#endif // GAME_CONTEXT_H
