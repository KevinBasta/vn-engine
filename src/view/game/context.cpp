
#include "shader.h"
#include "context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <utility>

#include "model_subject.h"


GameContext::GameContext(VnWindow* window, StateSubject* stateSubject) :
	m_window{ window },
	m_stateSubject{ stateSubject },
	m_backgroundLayer{ window, stateSubject },
	m_spriteLayer{ window, stateSubject },
	m_textLayer{ window, stateSubject },
	m_defaultShader{ TEMP_VERTEX_PATH, TEMP_FRAGMENT_PATH }
{
	// Uncomment to enable transparent backgrounds
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}




