#ifndef _H_RENDERER3D_
#define _H_RENDERER3D_
#include "Texture.h"
#include "ShaderProgram.h"
#include "RenderMesh.h"

class Renderer3D
{
public:
    // This struct is passed to the QueueRender function, mesh is mandatory
    // however, the user can choose to omit the program or vmat, to use the standard ones
    // provided by the renderer.
    struct RenderArgs
    {
        RenderMesh* mesh;
        ShaderProgram* prog;
        texture* tex;
        glm::mat4 vmat;
        glm::mat4 mmat;
        GLuint polymode;
        GLuint blendfunc_sfactor;
        GLuint blendfunc_dfactor;
        bool blend;
        bool backfacecull;
        RenderArgs()
        {
            mesh = nullptr;
            prog = nullptr;
            tex = nullptr;
            vmat = glm::mat4(0);
            mmat = glm::mat4(0);
            polymode = GL_FILL;
            blendfunc_sfactor = 0;
            blendfunc_dfactor = 0;
            blend = false;
            backfacecull = true;
        }
    };
    friend class Graphics;
    void QueueRender(const RenderArgs& args);
    void SetDefaultViewMatrix(const glm::mat4& nmat);
    inline glm::mat4 GetDefaultViewMatrix() {return currentVmat;}
private:
    void Init();
    void Render();
    float m_fov;
    float m_nplane;
    float m_fplane;
    glm::mat4 currentVmat;
    glm::mat4 lastVmat;
    ShaderProgram* rendProg;
    std::vector<RenderArgs> queuedRenders;
};
#endif