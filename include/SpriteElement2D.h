#ifndef _H_SPRITEEL2D_
#define _H_SPRITEEL2D_
#include "Element2D.h"
#include "ShaderProgram.h"
class SpriteElement2D : public Element2D
{
public:
    SpriteElement2D(const glm::vec2& pos, const glm::vec2& scale, texture* tex);
protected:
    void Render(ShaderProgram* shad);
    texture* m_tex;
};
#endif