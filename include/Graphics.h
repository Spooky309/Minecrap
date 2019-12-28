#ifndef _H_GRAPHICS_
#define _H_GRAPHICS_
#include "ShaderProgram.h"
#include "OpenGLW.h"
#include <glm/glm.hpp>
class Graphics
{
public:
    friend class Engine;
    inline GLFWwindow* GetWindow()
    {
        return wind;
    }
    inline ShaderProgram* GetRenderProgram()
    {
        return rendProg;
    }
    void SetRenderViewMatrix();
    void SetFOV(GLfloat fov);
    void SetNearPlane(GLfloat nPlane);
    void SetFarPlane(GLfloat fPlane);
    void RestartGraphics();
private:
    void Init();
    void Render();
    GLFWwindow* wind;
    ShaderProgram* rendProg;
    GLfloat m_fov, m_nplane, m_fplane;
};
#endif