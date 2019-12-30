#ifndef _H_FONTMANAGER_
#define _H_FONTMANAGER_
#include <vector>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"
class FontManager
{
public:
    FontManager();
    Font* LoadFont(const std::string& name);
private:
    std::vector<Font*> m_loadedFonts;
    FT_Library freeTypeLib;
};
#endif