#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Shader {
public:
	GLuint	m_programID{};
	const char*	m_vertexPath{};
	const char*	m_fragmentPath{};

public:
	/**
	 * Give two file paths, one for the vertex shader and one
	 * for the fragment shader, read, compile, and link them.
	 */
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader() { deleteShaderProgram(); };
	void use() const;
	void reload();
	GLuint ID() const { return m_programID; };


private:
	void compileShaderFiles();
	void checkCompileErrors(GLuint shader, std::string type);
	void checkLinkErrors(GLuint shader);
	void deleteShaderProgram();
};


#endif // OPENGL_SHADER_H
