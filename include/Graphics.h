#ifndef _H_GRAPHICS_
#define _H_GRAPHICS_
#include "ShaderProgram.h"
#include "OpenGLW.h"
#include <glm/glm.hpp>
#include "Renderer3D.h"
#include "Renderer2D.h"
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
    inline Renderer2D* Get2DRenderer() { return rend2D; }
private:
    void Init();
    void Render();
    GLFWwindow* wind;
    Renderer2D* rend2D;
    Renderer3D* rend3D;
};
#endif