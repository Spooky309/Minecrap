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
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Failed to load a glyph\n";
            continue;
        }
        SFChar thisc;
        glGenTextures(1, &thisc.textureID);
        glBindTexture(GL_TEXTURE_2D, thisc.textureID);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        thisc.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        thisc.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        thisc.advance = face->glyph->advance.x;
        Chars[c] = thisc;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}