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
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
    SetText(initText);
}

void TextElement2D::SetText(const std::string& newText)
{
    text = newText;
    if (text.length() == 0) return;
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Font::SubFont* sf = m_font->GetSize(textSize);
    std::string::const_iterator c;
    float x = m_pos.x + ((sf->GetChar('O')->advance.x >> 6) * m_scale.x);
    float ix = x;
    float y = m_pos.y - (18 * m_scale.y);
    GLfloat* verts = new GLfloat[text.length() * 30]; // 1 quad for each character
    size_t vertsi = 0;
    for (c = text.begin(); c < text.end(); c++)
    {
        Font::SubFont::SFChar* ch = sf->GetChar(*c);
        if (*c == '\n')
        {
            y -= (18) * m_scale.y;
            x = ix;
            continue;
        }
        GLfloat xpos = x + (ch->bearing.x * m_scale.x);
        GLfloat ypos = y - ((ch->size.y - ch->bearing.y) * m_scale.y);
        GLfloat w = ch->size.x * m_scale.x;
        GLfloat h = ch->size.y * m_scale.y;
        // top left side
        verts[vertsi++] = xpos;
        verts[vertsi++] = ypos+h;
        verts[vertsi++] = 0.0f;
        verts[vertsi++] = ch->coords.tLeft.x;
        verts[vertsi++] = ch->coords.tLeft.y;
        // bottom left side
        verts[vertsi++] = xpos;
        verts[vertsi++] = ypos;
        verts[vertsi++] = 0.0f;
        verts[vertsi++] = ch->coords.tLeft.x;
        verts[vertsi++] = ch->coords.bRight.y;
        // bottom right side
        verts[vertsi++] = xpos+w;
        verts[vertsi++] = ypos;
        verts[vertsi++] = 0.0f;
        verts[vertsi++] = ch->coords.bRight.x;
        verts[vertsi++] = ch->coords.bRight.y;
        // top left side
        verts[vertsi++] = xpos;
        verts[vertsi++] = ypos+h;
        verts[vertsi++] = 0.0f;
        verts[vertsi++] = ch->coords.tLeft.x;
        verts[vertsi++] = ch->coords.tLeft.y;
        // bottom right side
        verts[vertsi++] = xpos+w;
        verts[vertsi++] = ypos;
        verts[vertsi++] = 0.0f;
        verts[vertsi++] = ch->coords.bRight.x;
        verts[vertsi++] = ch->coords.bRight.y;
        // top right side
        verts[vertsi++] = xpos+w;
        verts[vertsi++] = ypos+h;
        verts[vertsi++] = 0.0f;
        verts[vertsi++] = ch->coords.bRight.x;
        verts[vertsi++] = ch->coords.tLeft.y;
        x += roundf((ch->advance.x >> 6) * m_scale.x);
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(30*text.length()), verts, GL_DYNAMIC_DRAW);
    delete[] verts;
}

void TextElement2D::Render(ShaderProgram*)
{
    if (text.length() == 0) return;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Font::SubFont* sf = m_font->GetSize(textSize);
    glBindTexture(GL_TEXTURE_2D, sf->textureID);
    glBindVertexArray(vao);
    myShad->Use();
    glDrawArrays(GL_TRIANGLES, 0, 6*text.length());
}