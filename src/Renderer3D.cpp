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

void Renderer3D::Render()
{
    glm::mat4 useMat = currentVmat;
    bool newMat = false;
    if (useMat != Engine::Instance().GetPlayer().GetViewMatrix())
    {
        useMat = Engine::Instance().GetPlayer().GetViewMatrix();
        currentVmat = useMat;
        rendProg->SetViewMatrix(useMat);
        newMat = true;
    }
    for (Renderer3D::RenderArgs r : queuedRenders)
    {
        ShaderProgram* curProg = rendProg;
        if (r.prog)
        {
            curProg = r.prog;
        }
        curProg->Use();
        if (r.prog && r.vmat != glm::mat4(0))
        {
            curProg->SetViewMatrix(r.vmat);
        }
        else if (r.prog && newMat)
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
        for (auto& it : r.mesh->GetVAOs())
        {
	        glBindVertexArray(it.first);
	        glDrawArrays(GL_TRIANGLES, 0, r.mesh->tCount);
        }
    }
}