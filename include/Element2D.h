#ifndef _H_2DELEMENT_
#define _H_2DELEMENT_
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"

class Element2D
{
public:
    Element2D(const glm::vec2& pos, const glm::vec2& scale);
    inline glm::vec2 GetPosition() {return m_pos;}
    void SetPosition(const glm::vec2& newp);
    void MovePosition(const glm::vec2& delta);
    inline glm::vec2 GetScale() {return m_scale;}
    void SetScale(const glm::vec2& news);
    void MoveScale(const glm::vec2& delta);
    friend class Renderer2D;
protected:
    glm::vec2 m_pos;
    glm::vec2 m_scale;
    virtual void Render(ShaderProgram* shad) = 0; // 2d objects define their own rendering logic... it's the only way
};
#endif