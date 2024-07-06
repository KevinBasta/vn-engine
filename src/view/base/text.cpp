
#include "text.h"

#include <memory>

std::unique_ptr<TextTexture> TextTexture::m_instance{ nullptr };

int TextTexture::initFreeType() {
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

	return 0;
}

void TextTexture::initVAO() {
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

int TextTexture::loadCharacter(const wchar_t charInput) {
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
	CharTextrueData character = {
		texture,
		glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
		glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
		static_cast<unsigned int>(m_face->glyph->advance.x)
	};

	m_loadedTextChars.insert(std::pair<wchar_t, CharTextrueData>(charInput, character));

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

