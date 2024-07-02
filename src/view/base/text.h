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
#include <memory>

#define TEMP_FONT_PATH				BASE_PATH"\\test\\assets\\BIZ-UDGothicB.ttc"

class TextTexture {
private:
	static std::unique_ptr<TextTexture> instance;

private:
	struct textChar {
		unsigned int textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	std::unordered_map<wchar_t, textChar> m_loadedTextChars{};

private: 
	FT_Library m_ft{};
	FT_Face m_face{};

	unsigned int m_VAO{};
	unsigned int m_VBO{};

	int initFreeType();
	void initVAO();

	int loadCharacter(const wchar_t charInput);

public:
	TextTexture()
	{
		initFreeType();
		initVAO();
	}

	~TextTexture() {
		// destroy FreeType once we're finished
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_ft);

		// TODO: Loop hashmap and delete textures
		// glDeleteTextures(1, &m_textureID);
		
		// TODO: delete VAO and VBO
		std::cout << "text manager deleted" << std::endl;
	}

public:

	static int computeBreakIndex(std::wstring_view text, int startIndex, int maxWidth, float scale) {
		if (instance.get() == nullptr) {
			instance = std::make_unique<TextTexture>();
		}

		int endIndex{ startIndex };
		int lastSpaceIndex{ startIndex };
		float x{ 0.0f };
		float y{ 0.0f };

		std::wstring_view::const_iterator c;
		for (c = text.begin() + startIndex; c != text.end(); c++)
		{
			if (instance.get()->m_loadedTextChars.find(*c) == instance.get()->m_loadedTextChars.end()) {
				instance.get()->loadCharacter(*c);
			}

			textChar ch = instance.get()->m_loadedTextChars[*c];

			if ((x + (ch.advance >> 6) * scale) > maxWidth) {
				if (*c != ' ') {
					endIndex = lastSpaceIndex;
				}

				break;
			}
			
			if (*c == '\n') {
				break;
			}

			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;
			
			x += (ch.advance >> 6) * scale;
			
			if (*c == ' ') {
				lastSpaceIndex = endIndex;
			}

			endIndex++;
		}

		//std::cout << "End index: " << endIndex << " " << std::endl;

		return endIndex;
	}

	// render line of text
	// -------------------
	static void draw(std::wstring_view text)
	{
		if (instance.get() == nullptr) {
			instance = std::make_unique<TextTexture>();
		}
		
		float scale{ 1.0f };
		float x{ 0.0f };
		float y{ 0.0f };
		
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(instance.get()->m_VAO);

		// iterate through all characters
		std::wstring_view::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			if (instance.get()->m_loadedTextChars.find(*c) == instance.get()->m_loadedTextChars.end()) {
				instance.get()->loadCharacter(*c);
				std::cout << "char loaded: " << static_cast<uint32_t>(*c) << std::endl;
			}

			textChar ch = instance.get()->m_loadedTextChars[*c];

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
			glBindBuffer(GL_ARRAY_BUFFER, instance.get()->m_VBO);
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
