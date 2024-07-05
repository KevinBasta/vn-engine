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

#define TEMP_FONT_PATH VN_BASE_PATH"/test/assets/BIZUDGothic-Bold.ttf"

class TextTexture {
private:
	static std::unique_ptr<TextTexture> m_instance;

	static void checkInstance() {
		if (m_instance.get() == nullptr) {
			m_instance = std::make_unique<TextTexture>();
		}
	}

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
	// TODO: can have differnet instances based on size of glyph
	// in menu can have "small", "medium", and "large" option for text
	// each corrosponds to a different instance

	// TODO: determine if different (multiple) instances (of the same size) can 
	// be used for multithreading

	/**
	 * Creates a vector of string views containing the exact substrings
	 */
	static std::vector<std::wstring_view> fitLineToScreen(std::wstring_view text, int maxWidth, float scale) {
		checkInstance();

		TextTexture* instance{ m_instance.get() };
		
		std::vector<std::wstring_view> fittedLines{};

		int startIndex{ 0 };
		int endIndex{ 0 };

		std::wstring_view::const_iterator c{ text.begin() };
		
		while (c != text.end()) {
			float x{ 0.0f };
			float y{ 0.0f };
			int lastSpaceIndex{ startIndex };
	
			if (c + 1 == text.end()) {
				break;
			}
			else {
				c++;
				endIndex++;
			}

			// Skip any non-display characters
			while (endIndex < text.length() - 1) {
				wchar_t currentChar{ text[endIndex] };

				if (currentChar == ' ' || currentChar == '\n') {
					endIndex++;
					if (c + 1 == text.end()) {
						break;
					}
					else {
						c++;
					}
				}
				else {
					break;
				}
			}

			for (; c != text.end(); c++)
			{
				if (instance->m_loadedTextChars.find(*c) == instance->m_loadedTextChars.end()) {
					instance->loadCharacter(*c);
				}

				textChar ch = instance->m_loadedTextChars[*c];

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

			fittedLines.push_back(text.substr(startIndex, endIndex));
			startIndex = endIndex;
		}

		for (auto line : fittedLines) {
			std::wcout << line << std::endl;
		}

		return fittedLines;
	}

	static int computeBreakIndex(std::wstring_view text, int startIndex, int maxWidth, float scale) {
		checkInstance();

		int endIndex{ startIndex };
		int lastSpaceIndex{ startIndex };
		float x{ 0.0f };
		float y{ 0.0f };

		std::wstring_view::const_iterator c;
		for (c = text.begin() + startIndex; c != text.end(); c++)
		{
			if (m_instance.get()->m_loadedTextChars.find(*c) == m_instance.get()->m_loadedTextChars.end()) {
				m_instance.get()->loadCharacter(*c);
			}

			textChar ch = m_instance.get()->m_loadedTextChars[*c];

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
		checkInstance();
		
		float scale{ 1.0f };
		float x{ 0.0f };
		float y{ 0.0f };
		
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_instance.get()->m_VAO);

		// iterate through all characters
		std::wstring_view::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			if (m_instance.get()->m_loadedTextChars.find(*c) == m_instance.get()->m_loadedTextChars.end()) {
				m_instance.get()->loadCharacter(*c);
				std::cout << "char loaded: " << static_cast<uint32_t>(*c) << std::endl;
			}

			textChar ch = m_instance.get()->m_loadedTextChars[*c];

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
			glBindBuffer(GL_ARRAY_BUFFER, m_instance.get()->m_VBO);
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

#endif //VN_TEXT_H
