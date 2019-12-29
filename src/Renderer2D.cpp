#include "Renderer2D.h"
#include "ShaderManager.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
void Renderer2D::Init()
{
    rendProg = ShaderManager::Instance().LoadShaderProgram("2d");
    rendProg->SetProjectionMatrix(glm::ortho(0.0f, 1024.0f, 768.0f, 0.0f));
}

void Renderer2D::QueueRender(Element2D* element)
{
    rendQueue.push_back(element);
}

void Renderer2D::Render()
{
    for (auto it : rendQueue)
    {
        glm::mat4 mod(1.0f);
        mod = glm::translate(mod, glm::vec3(it->GetPosition().x, it->GetPosition().y, 0.0f));
        mod = glm::scale(mod, glm::vec3(it->GetTexture()->width * it->GetScale().x,
            it->GetTexture()->height * it->GetScale().y, 1.0f));
        
        rendProg->Use();
        rendProg->SetModelMatrix(mod);
        glBindVertexArray(rendVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    rendQueue.clear();
}