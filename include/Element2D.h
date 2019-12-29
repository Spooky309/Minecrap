#ifndef _H_2DELEMENT_
#define _H_2DELEMENT_
#include <glm/glm.hpp>
#include "Texture.h"
class Element2D
{
public:
    inline glm::vec2 GetPosition() {return m_pos;}
    void SetPosition(const glm::vec2& newp);
    void MovePosition(const glm::vec2& delta);
    inline glm::vec2 GetScale() {return m_scale;}
    void SetScale(const glm::vec2& news);
    void MoveScale(const glm::vec2& delta);
    virtual texture* GetTexture() = 0; // this is to be overridden with the logic for calculating the texture of a 2d object
    // texture could be a literal sprite, or it could be calculated from some text... use your imagination.
private:
    glm::vec2 m_pos;
    glm::vec2 m_scale;
};
#endif