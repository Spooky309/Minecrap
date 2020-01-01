#include "Font.h"
#include <iostream>

Font::Font(const std::string& name, FT_Library ftLib)
{
    this->path = name;
    this->ftLib = ftLib;
    FT_New_Face(ftLib, name.c_str(), 0, &face);
}
Font::SubFont::SubFont(FT_Face& face, unsigned int size)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Set_Pixel_Sizes(face, 0, size);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    unsigned long long texWidth = 0;
    unsigned long long texHeight = 0;
    unsigned long long widestCh = 0;
    unsigned long long tallestCh = 0;
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Failed to load a glyph\n";
            continue;
        }
        if (face->glyph->bitmap.width > widestCh) widestCh = face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > tallestCh) tallestCh = face->glyph->bitmap.rows;
    }
    texWidth = widestCh * 32;
    texHeight = tallestCh * 4;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texWidth, texHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned long long x = 0;
    unsigned long long y = 0;
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Failed to load a glyph\n";
            continue;
        }
        SFChar thisc;
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            x,
            y,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        
        thisc.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        thisc.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        thisc.advance = glm::ivec2(face->glyph->advance.x, face->glyph->advance.y);
        SFUV uv = 
        {
            glm::vec2((float)(x)/(float)(texWidth), (float)(y)/(float)(texHeight)),
            glm::vec2((float)(x+face->glyph->bitmap.width)/(float)(texWidth), (float)(y+face->glyph->bitmap.rows)/(float)texHeight)
        };
        thisc.coords = uv;
        Chars[c] = thisc;
        if (x < 31 * widestCh)
        {
            x += widestCh;
        }
        else
        {
            x = 0;
            y += tallestCh;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}