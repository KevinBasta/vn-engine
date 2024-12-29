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

#define TEMP_FONT_PATH VN_BASE_PATH"\\test\\assets\\BIZUDGothic-Regular.ttf"

class TextTexture {
private:
	static std::unique_ptr<TextTexture> m_instance;

	static TextTexture* checkInstance() {
		if (m_instance.get() == nullptr) {
			m_instance = std::make_unique<TextTexture>();
		}

		return m_instance.get();
	}

private:
	struct CharTextrueData {
		unsigned int textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	std::unordered_map<wchar_t, CharTextrueData> m_loadedTextChars{};

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
	 * Return the amount of characters that can fit in a line of maxWidth from text using scale size.
	 * text			- The string to get a line substring from
	 * startIndex	- the index in text from which to get a line substring from
	 * maxWidth		- the maximum width (in pixels?) that a line can be in order to fit on screen
	 * scale		- the scale of the font being used
	 */
	static int computeBreakIndex(std::wstring_view text, int startIndex, int maxWidth, float scale) {
		TextTexture* instance{ checkInstance() };

		int charAmount{ 0 };
		int lastSpaceMarker{ 0 };
		float x{ 0.0f };

		std::wstring_view::const_iterator c;
		for (c = text.begin() + startIndex; c != text.end(); c++)
		{
			if (instance->m_loadedTextChars.find(*c) == instance->m_loadedTextChars.end()) {
				instance->loadCharacter(*c);
			}

			const CharTextrueData& ch{ instance->m_loadedTextChars[*c] };
			float newX = (x + (ch.advance >> 6) * scale);
			
			// If the line limit is reached and we're mid word,
			// go back to the end of the last word (the last space).
			if (newX > maxWidth && *c != L' ') {
				charAmount = lastSpaceMarker;
			}

			// If the line limit is reached or a new line
			// character is read return.
			if (newX > maxWidth  || *c == L'\n') {
				break;
			}

			x = newX;

			// Update the marker for the last time a word was finished
			// (a space was read).
			if (*c == L' ') {
				lastSpaceMarker = charAmount;
			}

			charAmount++;
		}

		return charAmount;
	}

	/**
	 * Creates a vector of string views containing the exact substrings
	 */
	static std::vector<std::wstring_view> fittedScreenLines(std::wstring_view text, int maxWidth, float scale) {
		TextTexture* instance{ checkInstance() };
		
		std::vector<std::wstring_view> fittedLines{};

		int startIndex{ 0 };
		int lineLength{ 0 };
		
		while (startIndex < text.length()) {

 			// Skip non-display characters at start of the new fitted line
			while (startIndex < text.length()) {
				wchar_t currentChar{ text[startIndex] };

				if (currentChar == L' ' || currentChar == L'\t' || currentChar == L'\n') { startIndex++; }
				else { break; }
			}

			lineLength = computeBreakIndex(text, startIndex, maxWidth, scale);

			if (lineLength <= 0) {
				// TODO: limitation. If a word spans the entire maxWidth it will result no more text
				// being displayed. Perhaps use a hyphen for separated words? will have to update the text
				// to be wstring type and insert a hyphen into the wstring for example.
				break;
			}

			fittedLines.push_back(text.substr(startIndex, lineLength));
			startIndex = startIndex + lineLength;
		}

		return fittedLines;
	}


	// render line of text
	// -------------------
	static void draw(std::wstring_view text)
	{
		TextTexture* instance{ checkInstance() };
		
		float scale{ 1.0f };
		float x{ 0.0f };
		float y{ 0.0f };
		
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(instance->m_VAO);

		// iterate through all characters
		std::wstring_view::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			if (instance->m_loadedTextChars.find(*c) == instance->m_loadedTextChars.end()) {
				instance->loadCharacter(*c);
				//std::cout << "char loaded: " << static_cast<uint32_t>(*c) << std::endl;
			}

			CharTextrueData& ch{ instance->m_loadedTextChars[*c] };

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
			glBindBuffer(GL_ARRAY_BUFFER, instance->m_VBO);
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
