
#include <iostream>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "runtime_characters.h"
#include "shader.h"
#include "frame.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

OpenGLCamera* g_camera{};

static void frameSizeUpdateCallback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	(*g_camera).processMouseMovement(xpos, ypos);
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	(*g_camera).processMouseScroll(yoffset);
}

void OpenGLFrame::initFrame() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(800, 600, "opengl :)", NULL, NULL);
	if (m_window == NULL) {
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		// raise exception or return err?
	}
	
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, frameSizeUpdateCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		// raise exception or return err?
	}

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	glViewport(0, 0, 800, 600);

	g_camera = &m_camera;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, mouseCallback);
	glfwSetScrollCallback(m_window, scrollCallback);
}


void OpenGLFrame::processInput() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}

	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		// reload all shaders?
	}

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_W, deltaTime);
	}

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_S, deltaTime);
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_D, deltaTime);
	}

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		m_camera.processKeyboard(GLFW_KEY_A, deltaTime);
	}
}

OpenGLFrame::OpenGLFrame() {
	initFrame();
}

void OpenGLFrame::gameLoop() {

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	OpenGLShader shader(
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Engine\\scene_vertex_shader.glsl",
		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\Engine\\scene_fragment_shader.glsl");

	float width		{ static_cast<float>(g_characters[0].get()->getTextures()[0].width())};
	float height	{ static_cast<float>(g_characters[0].get()->getTextures()[0].height())};

	float vertices[] = {
		// positions		 // texture coords
		 0.0f, height, 0.0f, 0.0f, 1.0f, // top left
		width, height, 0.0f, 1.0f, 1.0f, // top right
		 0.0f,   0.0f, 0.0f, 0.0f, 0.0f, // bottom left
		width,   0.0f, 0.0f, 1.0f, 0.0f, // bottom right
	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 2, 1
	};
	
	// vertex array object, vertex buffer object, Element Buffer Object
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Connect everything to the VAO
	glBindVertexArray(VAO);

	// vertex data buffer, put data in a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// define what points are used to make the shapes, reusing vertecies from VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// define how the different attributes data are stored

	// vertex coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture mapping
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	

	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID(), "inTexture"), 0);
	
	unsigned int modelLocation = glGetUniformLocation(shader.ID(), "inModel");
	unsigned int viewLocation = glGetUniformLocation(shader.ID(), "inView");
	unsigned int projectionLocation = glGetUniformLocation(shader.ID(), "inProjection");

	glm::mat4 projection = glm::perspective(90.0f, 800.0f / 600.0f, 0.001f, 1000.0f);


	while (!glfwWindowShouldClose(m_window)) {
		processInput();

		if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
			shader.reload();
			m_camera.resetCamera();
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);

		// set coordinate system matrix uniforms
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.0f));
		
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_camera.getViewMatrix()));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	glfwTerminate();
	// return?
}