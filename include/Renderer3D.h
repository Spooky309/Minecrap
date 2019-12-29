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
        RenderArgs(RenderMesh& m, ShaderProgram* p = nullptr, texture* t = nullptr, const glm::mat4& v = glm::mat4(0))
        {
            mesh = &m;
            prog = p;
            vmat = v;
            tex = t;
        }
    };
    friend class Graphics;
    void QueueRender(const RenderArgs& args);
private:
    void Init();
    void Render();
    float m_fov;
    float m_nplane;
    float m_fplane;
    glm::mat4 currentVmat;
    ShaderProgram* rendProg;
    std::vector<RenderArgs> queuedRenders;
};
#endif