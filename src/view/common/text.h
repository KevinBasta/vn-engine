#ifndef VN_TEXT_H
#define VN_TEXT_H

#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <iostream>
#include <unordered_map>
//#include <locale>
//#include < locale.h >

#define TEMP_FONT_PATH				"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\fonts\\BIZ-UDGothicB.ttc"
#define TEMP_TEXT_VERTEX_SHADER		"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\textVertex.glsl"
#define TEMP_TEXT_FRAGMENT_SHADER	"C:\\Users\\Kevin\\Documents\\CS\\cpp\\visual-novel-engine\\visual_novel_engine\\src\\view\\glsl\\textFragment.glsl"

struct textChar {
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class VnText {
private: 
	std::unordered_map<wchar_t, textChar> m_loadedTextChars{};
	Shader	m_textShader;
	FT_Library m_ft{};
	FT_Face m_face{};
	unsigned int m_VAO{}; 
	unsigned int m_VBO{};


	int initFreeType() {
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&m_ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return -1;
		}

		// load font as face
		if (FT_New_Face(m_ft, TEMP_FONT_PATH, 0, &m_face)) {
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			return -1;
		}
	}

	void initVAO() {
		// configure VAO/VBO for texture quads
		// -----------------------------------

		// Generate buffers
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		// Bind buffers
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	int loadCharacter(const wchar_t charInput) {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(m_face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//FT_Select_Charmap(m_face, ft_encoding_unicode);
		unsigned long c = FT_Get_Char_Index(m_face, charInput);

		// Load character glyph 
		if (FT_Load_Glyph(m_face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			return -1;
		}

		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);

		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// now store character for later use
		textChar character = {
			texture,
			glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
			glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
			static_cast<unsigned int>(m_face->glyph->advance.x)
		};

		m_loadedTextChars.insert(std::pair<wchar_t, textChar>(charInput, character));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

public:
	VnText() : 
		m_textShader{ TEMP_TEXT_VERTEX_SHADER, TEMP_TEXT_FRAGMENT_SHADER }
	{
		initFreeType();
		initVAO();
	}

	~VnText() {
		// destroy FreeType once we're finished
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_ft);
	}

	// render line of text
	// -------------------
	void RenderText(std::wstring text, float x, float y, float scale, glm::vec3 color)
	{
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// activate corresponding render state	
		m_textShader.use();
		glUniform3f(glGetUniformLocation(m_textShader.ID(), "inTextColor"), color.x, color.y, color.z);

		float scalee{ 0.001f };
		float scaledWidth{ 100 * scalee };
		float scaledHeight{ 10 * scalee };

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0 * (scaledWidth / 2.0f), -1.0 * (scaledHeight / 2.0f), -1.0f));
		model = glm::scale(model, glm::vec3(scalee, scalee, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(m_textShader.ID(), "inModel"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_VAO);

		// iterate through all characters
		std::wstring::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			if (m_loadedTextChars.find(*c) == m_loadedTextChars.end()) {
				loadCharacter(*c);
				std::cout << "char loaded: " << static_cast<uint32_t>(*c) << std::endl;
			}

			textChar ch = m_loadedTextChars[*c];

			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

};

#endif VN_TEXT_H
