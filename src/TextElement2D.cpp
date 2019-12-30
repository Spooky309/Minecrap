#include "TextElement2D.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
TextElement2D::TextElement2D(const glm::vec2& pos, const glm::vec2& scale, Font* font, std::string initText):
    Element2D(pos, scale), text(initText)
{
    m_font = font;
    myShad = ShaderManager::Instance().LoadShaderProgram("text");
    myShad->SetProjectionMatrix(Engine::Instance().GetGraphics().Get2DRenderer()->GetProjectionMatrix());
}

void TextElement2D::Render(ShaderProgram* shad)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    Font::SubFont* sf = m_font->GetSize(42);
    std::string::const_iterator c;
    float x = m_pos.x + ((sf->GetChar(text[0])->advance >> 6) * m_scale.x);
    float ix = x;
    float y = m_pos.y;
    myShad->Use();
    for (c = text.begin(); c < text.end(); c++)
    {
        if (*c == '\n')
        {
            y += (sf->GetChar('A')->size.y * 1.5f) * m_scale.y;
            x = ix;
            continue;
        }
        Font::SubFont::SFChar* ch = sf->GetChar(*c);
        GLfloat xpos = x + ch->bearing.x * m_scale.x;
        GLfloat ypos = y + ch->bearing.y * m_scale.y;
        GLfloat w = ch->size.x * m_scale.x;
        GLfloat h = ch->size.y * m_scale.y;
        glm::mat4 mod(1.0f);
        mod = glm::translate(mod, glm::vec3(xpos,ypos,0.0f));
        mod = glm::scale(mod, glm::vec3(w,h,1.0f));
        myShad->SetModelMatrix(mod);
        glBindTexture(GL_TEXTURE_2D, ch->textureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch->advance >> 6) * m_scale.x;
    }
}