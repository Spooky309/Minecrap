#include "Renderer3D.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
void Renderer3D::QueueRender(const RenderArgs& args)
{
    queuedRenders.push_back(args);
}

void Renderer3D::Init()
{
    m_fov = 50.0f;
    m_nplane = 0.1f;
    m_fplane = 1000.0f;
    rendProg = ShaderManager::Instance().LoadShaderProgram("block");
    rendProg->SetProjectionMatrix(glm::perspective(glm::radians(m_fov), 1024.0f/768.0f, m_nplane, m_fplane));
}
void Renderer3D::SetDefaultViewMatrix(const glm::mat4& nmat)
{
    currentVmat = nmat;
}
void Renderer3D::Render()
{
    glm::mat4 useMat = currentVmat;
    bool newMat = false;
    if (useMat != lastVmat)
    {
        useMat = currentVmat;
        rendProg->SetViewMatrix(useMat);
        lastVmat = useMat;
        newMat = true;
    }
    ShaderProgram* curProg;
    ShaderProgram* lastProg;
    for (Renderer3D::RenderArgs& r : queuedRenders)
    {
        curProg = rendProg;
        if (r.prog)
        {
            curProg = r.prog;
        }
        if (curProg != lastProg)
        {
            curProg->Use();
        }
        if (r.vmat != glm::mat4(0))
        {
            curProg->SetViewMatrix(r.vmat);
        }
        else if (curProg != lastProg)
        {
            curProg->SetViewMatrix(useMat);
        }
        if (r.tex)
        {
            glBindTexture(GL_TEXTURE_2D, r.tex->glTex);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, Engine::Instance().GetTextureDictionary().GetTexture().glTex);
        }
        glPolygonMode(GL_FRONT_AND_BACK, r.polymode);
        if (r.blend)
        {
            glEnable(GL_BLEND);
            glBlendFunc(r.blendfunc_sfactor, r.blendfunc_dfactor);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        if (r.backfacecull)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        if (r.mmat != glm::mat4(0))
        {
            curProg->SetModelMatrix(r.mmat);
        }
        for (auto& it : r.mesh->GetVAOs())
        {
	        glBindVertexArray(it.first);
	        glDrawArrays(GL_TRIANGLES, 0, r.mesh->tCount);
        }
        lastProg = curProg;
    }
    queuedRenders.clear();
    // make sure defaults are restored at the end
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}