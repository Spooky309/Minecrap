#ifndef _H_FONT_
#define _H_FONT_
#include <string>
#include <map>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "OpenGLW.h"
#include <glm/glm.hpp>
class Font
{
public:
    class SubFont
    {
    public:
        struct SFChar
        {
            GLuint textureID;
            glm::ivec2 size;
            glm::ivec2 bearing;
            GLuint advance;
        };
        SubFont(FT_Face& face, unsigned int size);
        inline SFChar* GetChar(unsigned char c)
        {
            return &Chars[c];
        }
    private:
        SFChar Chars[128];
    };
    Font(const std::string& path, FT_Library ftLib);
    inline SubFont* GetSize(unsigned int size)
    {
        auto it = subFonts.find(size);
        if (it != subFonts.end())
        {
            return it->second;
        }
        SubFont* nf = new SubFont(face, size);
        subFonts.emplace(std::pair<unsigned int, SubFont*>(size, nf));
        return nf;
    }
    friend class FontManager;
private:
    std::string path;
    std::map<unsigned int, SubFont*> subFonts;
    FT_Library ftLib;
    FT_Face face;
};
#endif