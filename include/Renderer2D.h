#ifndef _H_RENDERER_2D
#define _H_RENDERER_2D
#include "OpenGLW.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Element2D.h"
class Renderer2D
{
public:
    void QueueRender(Element2D* element);
    inline glm::mat4 GetProjectionMatrix()
    {
        return rendProg->GetProjectionMatrix();
    }
    friend class Graphics;
private:
    void Init();
    void Render();
    Element2D** rQ;
    size_t rqLen;
    ShaderProgram* rendProg;
    GLuint rendVBO;
    GLuint rendVAO;
    GLuint scratchVBO;
    GLuint scratchVAO;
};
#endif