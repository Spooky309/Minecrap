#include "Element2D.h"
Element2D::Element2D(const glm::vec2& pos, const glm::vec2& scale)
{
    m_pos = pos;
    m_scale = scale;
}
void Element2D::SetPosition(const glm::vec2& newp)
{
    m_pos = newp;
}
void Element2D::MovePosition(const glm::vec2& delta)
{
    m_pos += delta;
}
void Element2D::SetScale(const glm::vec2& news)
{
    m_scale = news;
}
void Element2D::MoveScale(const glm::vec2& delta)
{
    m_scale += delta;
}