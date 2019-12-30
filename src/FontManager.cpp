#include "FontManager.h"
#include "Engine.h"
FontManager::FontManager()
{
    if (FT_Init_FreeType(&freeTypeLib))
    {
        std::cout << "Couldn't init FreeType\n";
    }
}
Font* FontManager::LoadFont(const std::string& name)
{
    std::string fpath = Engine::Instance().GetFileSystem().GetAbsPathTo(name);
    for (Font* f : m_loadedFonts)
    {
        if (f->path == fpath)
        {
            return f;
        }
    }
    return m_loadedFonts.emplace_back(new Font(fpath, freeTypeLib));
}