#include "SpriteElement2D.h"
#include <glm/gtx/transform.hpp>
SpriteElement2D::SpriteElement2D(const glm::vec2& pos, const glm::vec2& scale, texture* tex)
: Element2D(pos, scale)
{
    m_tex = tex;
}

void SpriteElement2D::Render(ShaderProgram* prog)
{
    glm::mat4 mmat = glm::mat4(1.0f);
    mmat = glm::translate(mmat, glm::vec3(m_pos.x, m_pos.y, 0.0f));
    mmat = glm::scale(mmat, glm::vec3(m_tex->width * m_scale.x,m_tex->height * m_scale.y, 0.0f));
    prog->SetModelMatrix(mmat);
    glBindTexture(GL_TEXTURE_2D, m_tex->glTex);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}