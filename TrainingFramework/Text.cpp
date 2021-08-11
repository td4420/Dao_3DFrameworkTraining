#include"stdafx.h"
#include"Text.h"
#include"Globals.h"
#include<iostream>
void Text::init() {
	if (FT_Init_FreeType(&m_ft))
	{
		printf("Could not init freetype library\n");
		return;
	}
	if (FT_New_Face(m_ft, fileFont, 0, &m_face)) {
		printf("Could not open font Olympus Mount.ttf\n");
		return;
	}
	FT_Set_Pixel_Sizes(m_face, 0, size);
	m_glyphSlot = m_face->glyph;
	for (const char* p = text; *p; p++)
	{
		if (FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
		{
			continue;
		}
		float sx = 1.0f / Globals::screenWidth * scaleX;
		float sy = 1.0f / Globals::screenHeight * scaleY;
		float w = m_glyphSlot->bitmap.width * sx;
		float h = m_glyphSlot->bitmap.rows * sy;
		widthText += w/2*Globals::screenWidth;
		heightText = h/2*Globals::screenHeight;
	}
	widthText += 14.5;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenBuffers(1, &textVBO);
	glGenBuffers(1, &textImgTexture);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBindBuffer(GL_TEXTURE_2D, textImgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::RenderText(Shaders shader) {
	// use glProgram, bind texture & pass color uniform here
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader.program);
	glUniform4f(glGetUniformLocation(shader.program, "u_color"), color.x, color.y, color.z, color.w);

	//glGenBuffers(1, &textVBO);
	//glGenBuffers(1, &textImgTexture);

	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBindBuffer(GL_TEXTURE_2D, textImgTexture);

	float sx = 1.0f / Globals::screenWidth * scaleX;
	float sy = 1.0f / Globals::screenHeight * scaleY;
	float x = posX;
	float y = posY;
	x = -1.0f + 2.0f * x / Globals::screenWidth;
	y = -1.0f + 2.0f * y / Globals::screenHeight;
	FT_Set_Pixel_Sizes(m_face, 0, size);
	for (const char* p = text; *p; p++)
	{
		if (FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
		{
			continue;
		}
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			m_glyphSlot->bitmap.width,
			m_glyphSlot->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			m_glyphSlot->bitmap.buffer
		);
		float x2 = x + m_glyphSlot->bitmap_left * sx;
		float y2 = y + m_glyphSlot->bitmap_top * sy;
		float w = m_glyphSlot->bitmap.width * sx;
		float h = m_glyphSlot->bitmap.rows * sy;
		GLfloat box[4][4] = {
		{x2, y2 , 0, 0},
		{x2 + w, y2 , 1, 0},
		{x2, y2 - h, 0, 1},
		{x2 + w, y2 - h, 1, 1},
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		if (glGetAttribLocation(shader.program, "a_position") != -1)
		{
			glEnableVertexAttribArray(glGetAttribLocation(shader.program, "a_position"));
			glVertexAttribPointer(glGetAttribLocation(shader.program, "a_position"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		}

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		x += (m_glyphSlot->advance.x >> 6) * sx;
		y += (m_glyphSlot->advance.y >> 6) * sy;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBlendFunc(GL_ONE, GL_ZERO);
	glDisable(GL_BLEND);

}
bool Text::checkChoose(int x, int y) {
	float subY = Globals::screenHeight - posY;
	if (x >= posX && x <= posX + widthText && y >= subY - heightText && y <= subY) {
		isChoose = true;
		return true;
	}
	else isChoose = false;
	return false;
}
void Text::highLight() {
	if (isChoose) {
		color = Vector4(0.6,1,0.4,1);
	}
	if (isChoose==false) color = Vector4(0.5,0.5,0.5,0.5); 
}
Text::Text(const char* s, float x, float y, const char* fileFont, float scaleX, float scaleY, Vector4 color, int size) {
	text = strdup(s);
	this->posX = x;
	this->posY = y;
	this->heightText = 0;
	this->widthText = 0;
	this->color = color;
	this->fileFont = strdup(fileFont);
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->size = size;
}
Text::~Text() {

}
