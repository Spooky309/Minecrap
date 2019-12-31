#include "TextElement2D.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
TextElement2D::TextElement2D(const glm::vec2& pos, const glm::vec2& scale, Font* font, std::string initText):
    Element2D(pos, scale), text(initText)
{
    m_useScratchVAO = true;
    m_font = font;
    myShad = ShaderManager::Instance().LoadShaderProgram("text");
    myShad->SetProjectionMatrix(Engine::Instance().GetGraphics().Get2DRenderer()->GetProjectionMatrix());
}

void TextElement2D::Render(ShaderProgram*)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    Font::SubFont* sf = m_font->GetSize(18);
    std::string::const_iterator c;
    float x = m_pos.x + ((sf->GetChar('O')->advance.x >> 6) * m_scale.x);
    float ix = x;
    float y = m_pos.y - ((sf->GetChar('O')->size.y * 2.0f) * m_scale.y);
    myShad->Use();
    for (c = text.begin(); c < text.end(); c++)
    {
        Font::SubFont::SFChar* ch = sf->GetChar(*c);
        if (*c == '\n')
        {
            y -= (sf->GetChar('O')->size.y * 2.0f) * m_scale.y;
            x = ix;
            continue;
        }
        GLfloat xpos = x + (ch->bearing.x * m_scale.x);
        GLfloat ypos = y - ((ch->size.y - ch->bearing.y) * m_scale.y);
        GLfloat w = ch->size.x * m_scale.x;
        GLfloat h = ch->size.y * m_scale.y;
        GLfloat verts[30] =
        {
            xpos,   ypos+h, 0.0f, 0.0f, 0.0f,
            xpos,   ypos,   0.0f, 0.0f, 1.0f,
            xpos+w, ypos,   0.0f, 1.0f, 1.0f,
            xpos,   ypos+h, 0.0f, 0.0f, 0.0f,
            xpos+w, ypos,   0.0f, 1.0f, 1.0f,
            xpos+w, ypos+h, 0.0f, 1.0f, 0.0f
            
        };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*30, verts);
        glBindTexture(GL_TEXTURE_2D, ch->textureID);
        glDisable(GL_CULL_FACE);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_CULL_FACE);
        x += roundf((ch->advance.x >> 6) * m_scale.x);
    }
}