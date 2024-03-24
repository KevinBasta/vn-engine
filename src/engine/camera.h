#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FlyCamera {
private:
	static constexpr glm::vec3 defaultPosition	{ glm::vec3(0.0f, 0.0f, 0.0f) };
	static constexpr glm::vec3 defaultFront		{ glm::vec3(0.0f, 0.0f, -1.0f) };
	static constexpr glm::vec3 defaultWorldUp	{ glm::vec3(0.0f, 1.0f, 0.0f) };

	static constexpr float defaultYaw			{ -90.0f };
	static constexpr float defaultPitch			{ 0.0f };
	static constexpr float defaultSpeed			{ 0.005f };
	static constexpr float defaultSensitivity	{ 0.1f };
	static constexpr float defaultZoom			{ 45.0f };

private:
	// camera attributes
	glm::vec3 m_position{};
	glm::vec3 m_front{};
	glm::vec3 m_up{};
	glm::vec3 m_right{};
	glm::vec3 m_worldUp{};

	// euler angles
	float m_yaw{};
	float m_pitch{};
	
	bool m_firstMouse{ true };
	float m_lastX{};
	float m_lastY{};

	// camera options
	float m_movementSpeed{};
	float m_mouseSensitivity{};
	float m_zoom{};

public:
	FlyCamera();
	void resetCamera();
	glm::mat4 getViewMatrix();
	
	void processKeyboard(int glfwKey, float deltaTime);
	void processMouseMovement(float xpos, float ypos);
	void processMouseScroll(float yoffset);

private:
	void updateCameraVectors();
};


#endif // OPENGL_CAMERA_H
