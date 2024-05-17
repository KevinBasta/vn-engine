#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "model_subject.h"

class GameContext {
public:
	GLFWwindow* m_window{};
	FlyCamera m_camera{};
	float m_frameWidth{ 800.0f };
	float m_frameHeight{ 600.0f };

	void initFrame();
	void processInput();

	//void updateResolution();

	ModelSubject* m_subject{};

public:
	GameContext();
	void addSubject(ModelSubject* subject) {
		m_subject = subject;
	}
	void gameLoop();
};

#endif // GAME_CONTEXT_H
