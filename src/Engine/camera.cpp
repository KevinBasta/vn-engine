#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

OpenGLCamera::OpenGLCamera() :
	m_position{ defaultPosition },
	m_front{ defaultFront },
	m_worldUp{ defaultWorldUp },
	m_yaw{ defaultYaw },
	m_pitch{ defaultPitch },
	m_movementSpeed{ defaultSpeed },
	m_mouseSensitivity{ defaultSensitivity },
	m_zoom{ defaultZoom }
{
	updateCameraVectors();
}

void OpenGLCamera::resetCamera() {
	m_position = defaultPosition;
	m_front = defaultFront;
	m_worldUp = defaultWorldUp;

	m_yaw = defaultYaw;
	m_pitch = defaultPitch;

	updateCameraVectors();
}

void OpenGLCamera::updateCameraVectors() {
	glm::vec3 front{};
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

// Return the vertex shader camera view matrix
glm::mat4 OpenGLCamera::getViewMatrix() {
	return glm::lookAt(m_position, m_position + m_front, m_up);
}




void OpenGLCamera::processKeyboard(int glfwKey, float deltaTime) {
	float velocity = m_movementSpeed * deltaTime;
	
	// Front
	if (glfwKey == GLFW_KEY_W) {
		m_position += m_front * velocity;
	}

	// Back
	if (glfwKey == GLFW_KEY_S) {
		m_position -= m_front * velocity;
	}
	
	// Left
	if (glfwKey == GLFW_KEY_A) {
		m_position -= m_right * velocity;
	}

	// Right
	if (glfwKey == GLFW_KEY_D) {
		m_position += m_right * velocity;
	}
}

void OpenGLCamera::processMouseMovement(float xpos, float ypos) {
	if (m_firstMouse) {
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xoffset{ xpos - m_lastX };
	float yoffset{ m_lastY - ypos };
	m_lastX = xpos;
	m_lastY = ypos;

	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	// don't let screen get flipped 
	if (m_pitch > 89.0f) {
		m_pitch = 89.0f;
	}

	if (m_pitch < -89.0f) {
		m_pitch = -89.0f;
	}

	// update up, right, front
	updateCameraVectors();
}

void OpenGLCamera::processMouseScroll(float yoffset) {
	m_zoom -= yoffset;
	if (m_zoom < 1.0f)
		m_zoom = 1.0f;
	if (m_zoom > 45.0f)
		m_zoom = 45.0f;
}

