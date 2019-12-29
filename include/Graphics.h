#ifndef _H_GRAPHICS_
#define _H_GRAPHICS_
#include "ShaderProgram.h"
#include "OpenGLW.h"
#include <glm/glm.hpp>
#include "Renderer3D.h"
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
        return rend3D->rendProg;
    }
    void SetRenderViewMatrix();
    void SetFOV(GLfloat fov);
    void SetNearPlane(GLfloat nPlane);
    void SetFarPlane(GLfloat fPlane);
    void RestartGraphics();
    inline Renderer3D* Get3DRenderer() { return rend3D; }
private:
    void Init();
    void Render();
    GLFWwindow* wind;
    Renderer3D* rend3D;
};
#endif