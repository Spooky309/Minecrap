#ifndef _H_TEXTEL2D_
#define _H_TEXTEL2D_
#include "Element2D.h"
#include "ShaderProgram.h"
#include "Font.h"
#include <string>

class TextElement2D : public Element2D
{
public:
    inline void SetText(std::string newText){text = newText;}
    TextElement2D(const glm::vec2& pos, const glm::vec2& scale, Font* font, std::string initText);
protected:
    void Render(ShaderProgram* shad);
    Font* m_font;
    std::string text;
    ShaderProgram* myShad;
};
#endif