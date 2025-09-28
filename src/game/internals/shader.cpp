
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath) :
	m_vertexPath{ vertexPath },
	m_fragmentPath{ fragmentPath }
{
}

void Shader::checkCompiled() {
	if (!m_compiled) {
		compileShaderFiles();
		m_compiled = true;
	}
}

void Shader::reload() {
	compileShaderFiles();
}

void Shader::use() {
	checkCompiled();
	glUseProgram(m_programID);
}

GLuint Shader::ID() { 
	checkCompiled();  
	return m_programID; 
};

void Shader::compileShaderFiles() {
	// 1: Get the shader code from files
	std::string vertexCode{};
	std::string fragmentCode{};

	std::ifstream vertexShaderFile{};
	std::ifstream fragmentShaderFile{};

	// allow ifstream objects to throw exceptions
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vertexShaderFile.open(m_vertexPath);
		fragmentShaderFile.open(m_fragmentPath);

		// read the files buffer contents into streams
		std::stringstream vertexShaderStream{};
		std::stringstream fragmentShaderStream{};
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// convert streams into strings
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "Shader file load error for " << m_vertexPath << std::endl;
		std::cout << "Shader file load error for " << m_fragmentPath << std::endl;
		std::cout << "ERROR::SHADER_FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}


	// 2: Compile the shaders
	const char* vertexShaderCode{ vertexCode.c_str() };
	const char* fragmentShaderCode{ fragmentCode.c_str() };

	GLuint vertexShader;
	GLuint fragmentShader;

	// vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	// fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");


	// 3: Link the shaders
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	checkLinkErrors(m_programID);


	// 4: Delete the shaders are they are linked in the program
	// TODO: check return value
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
	GLint success;
	GLchar infoLog[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		std::cout << m_vertexPath << std::endl;
		std::cout << m_fragmentPath << std::endl;
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "---------------------------------------------------" << std::endl;
		std::cout << "ERROR::SHADER_COMPILATION_ERROR::" << type << std::endl;
		std::cout << infoLog << std::endl;
		std::cout << "---------------------------------------------------" << std::endl;
	}
}

void Shader::checkLinkErrors(GLuint shader) {
	GLint success;
	GLchar infoLog[1024];

	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if (!success) {
		std::cout << m_vertexPath << std::endl;
		std::cout << m_fragmentPath << std::endl;
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "---------------------------------------------------" << std::endl;
		std::cout << "ERROR::PROGRAM_LINKING_ERROR" << std::endl;
		std::cout << infoLog << std::endl;
		std::cout << "---------------------------------------------------" << std::endl;
	}
}

void Shader::deleteShaderProgram() {
	std::cout << "delete shader" << std::endl;

	// TODO: check return value
	glDeleteProgram(m_programID);
}